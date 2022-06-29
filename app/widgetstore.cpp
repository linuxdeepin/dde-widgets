/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     yeshanshan <yeshanshan@uniontech.com>
 *
 * Maintainer: yeshanshan <yeshanshan@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "widgetstore.h"
#include "pluginspec.h"
#include "widgethandler.h"
#include "widgetmanager.h"
#include "instanceproxy.h"
#include "utils.h"

#include <QScrollArea>
#include <QDebug>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMoveEvent>
#include <QMimeData>
#include <QDrag>
#include <QLabel>
#include <QStackedLayout>

#include <DIconButton>
#include <DAnchors>
#include <DFontSizeManager>
#include <DButtonBox>

WIDGETS_FRAME_BEGIN_NAMESPACE
WidgetStore::WidgetStore(WidgetManager *manager, QWidget *parent)
    : QWidget(parent)
    , m_manager(manager)
    , m_views(new QWidget(this))
    , m_layout(new QVBoxLayout(m_views))
{
    m_layout->setSpacing(UI::Store::spacing);

    QPalette pt = palette();
    pt.setColor(QPalette::Window, Qt::transparent);
    setPalette(pt);
    setAutoFillBackground(true);

    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(UI::defaultMargins);
    layout->setSpacing(UI::Store::spacing);

    layout->addWidget(m_views);
    layout->addStretch();
}

void WidgetStore::init()
{
    m_views->setFixedWidth(width());
    m_views->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    load();
}

QScrollArea *WidgetStore::scrollView()
{
    if (!m_scrollView) {
        auto scrollArea = new QScrollArea();

        QPalette pt = scrollArea->palette();
        pt.setColor(QPalette::Window, Qt::transparent);
        scrollArea->setPalette(pt);
        scrollArea->setWidget(this);
        scrollArea->setWidgetResizable(true);
        scrollArea->setAutoFillBackground(true);
        scrollArea->setFrameStyle(QFrame::NoFrame);
        m_scrollView = scrollArea;
    }

    return m_scrollView;
}

void WidgetStore::addPlugin(const PluginId &pluginId)
{
    auto pluginCell = addPluginCell(pluginId);
    m_layout->addWidget(pluginCell);
    m_pluginCells.insert(pluginId, pluginCell);
}

PluginCell *WidgetStore::addPluginCell(const PluginId &pluginId)
{
    auto plugin = m_manager->getPlugin(pluginId);
    auto pluginCell = new PluginCell(this);
    pluginCell->setTitle(plugin->title());
    pluginCell->setDescription(plugin->description());
    for (auto instance: m_manager->createWidgetStoreInstances(pluginId)) {
        auto view = instance->view();
        Q_ASSERT(view);
        auto cell = new WidgetStoreCell(instance->handler(), this);
        connect(cell, &WidgetStoreCell::addWidget, this, &WidgetStore::addWidget);
        cell->setView(view);
        pluginCell->addCell(cell);
    }
    const int selectedCell = 0;
    pluginCell->setChecked(selectedCell);
    return pluginCell;
}

void WidgetStore::removePlugin(const PluginId &pluginId)
{
    for (auto iter = m_pluginCells.begin(); iter != m_pluginCells.end();) {
        if (iter.key() == pluginId) {
            m_layout->removeWidget(iter.value());
            iter.value()->deleteLater();
            iter = m_pluginCells.erase(iter);
        } else {
            iter++;
        }
    }
}

void WidgetStore::load()
{
    const auto plugins = m_manager->plugins(IWidgetPlugin::Normal);
    for (auto plugin : plugins) {
        const auto pluginId = plugin->id();
        addPlugin(pluginId);
    }
    m_layout->addStretch();
}

PluginCell::PluginCell(QWidget *parent)
    : DBlurEffectWidget (parent)
{
    setBlurRectXRadius(UI::Store::pluginCellRadius);
    setBlurRectYRadius(UI::Store::pluginCellRadius);
    setMaskColor(UI::Store::cellBackgroundColor);

    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(UI::Store::leftMargin, UI::Store::topMargin, UI::Store::rightMargin, UI::Store::bottomMargin);
    layout->setAlignment(Qt::AlignHCenter);
    m_title = new QLabel();
    m_title->setObjectName("store-title-label");
    {
        QFont font;
        font.setBold(true);
        m_title->setFont(DFontSizeManager::instance()->t5(font));
    }
    m_title->setAlignment(Qt::AlignHCenter);
    layout->addWidget(m_title);
    m_description = new QLabel();
    m_title->setObjectName("store-description-label");
    m_description->setAlignment(Qt::AlignHCenter);
    {
        m_description->setFont(DFontSizeManager::instance()->t8());
    }

    QWidget *views = new QWidget();
    auto viewsLayout = new QVBoxLayout(views);
    auto cellViews = new QWidget(views);

    m_layout = new QStackedLayout(cellViews);
    m_typeBox = new DButtonBox;
    m_typeBox->setFixedSize(UI::Store::TypeBoxSize);
    viewsLayout->addWidget(m_typeBox, 0, Qt::AlignHCenter);
    viewsLayout->addWidget(cellViews, 0, Qt::AlignHCenter);
    connect(m_typeBox, &DButtonBox::buttonClicked, this, [this](QAbstractButton *btn) {
        const auto index = m_typeBox->id(btn);
        Q_ASSERT(index >= 0 && index < m_layout->count());

        m_layout->setCurrentIndex(index);
    });

    layout->addWidget(views, 0, Qt::AlignHCenter);
    layout->addWidget(m_description);
    layout->addStretch();
}

void PluginCell::setTitle(const QString &text)
{
    m_title->setText(text);
}

void PluginCell::setDescription(const QString &text)
{
    m_description->setText(text);
}

void PluginCell::addCell(WidgetStoreCell *cell)
{
    const auto text= WidgetHandlerImpl::get(cell->m_handler)->typeString();
    auto btn = new DButtonBoxButton(text, cell);
    btn->setText(text);
    QList<DButtonBoxButton *> tmpBtnList;
    for (auto item : m_typeBox->buttonList()) {
        tmpBtnList << qobject_cast<DButtonBoxButton *>(item);
    }
    tmpBtnList << btn;
    m_typeBox->setButtonList(tmpBtnList, true);
    for (int i = 0; i < tmpBtnList.count(); ++i) {
        m_typeBox->setId(tmpBtnList[i], i);
    }
    // put in a layout because of different cell's size.
    auto cellView = new QWidget();
    auto cellLayout = new QVBoxLayout(cellView);
    cellLayout->addWidget(cell, 0, Qt::AlignHCenter);
    m_layout->addWidget(cellView);
}

void PluginCell::setChecked(const int index, const bool checked)
{
    if(index < 0 || index >= m_typeBox->buttonList().count())
        return;

    m_typeBox->buttonList()[index]->click();
}

WidgetStoreCell::WidgetStoreCell(WidgetHandler *handler, QWidget *parent)
    : DragDropWidget(parent)
    , m_handler(handler)
{
    setFocusPolicy(Qt::NoFocus);
}

void WidgetStoreCell::setView(QWidget *view)
{
    m_view = view;
    m_view->setParent(this);

    auto action = new DIconButton(DStyle::SP_AddButton);
    action->setObjectName("add-button");
    action->setParent(this);
    action->setIconSize(UI::Store::AddIconSize);
    action->setFlat(true);
    action->setVisible(false);
    connect(this, &WidgetStoreCell::enterChanged, action, &QWidget::setVisible);
    connect(action, &DIconButton::clicked, this, [this](){
        Q_EMIT addWidget(m_handler->pluginId(), m_handler->type());
    });

    auto cellAnchors = new DAnchors<WidgetStoreCell>(this);
    auto actionAnchors = new DAnchors<DIconButton>(action);
    auto viewAnchors = new DAnchors<QWidget>(m_view);
    actionAnchors->setTop(cellAnchors->top());
    actionAnchors->setRight(cellAnchors->right());
    viewAnchors->setBottom(cellAnchors->bottom());

    setFixedSize(m_handler->size() + (UI::Store::AddIconSize / 2));
}

void WidgetStoreCell::startDrag(const QPoint &pos)
{
    QWidget *child = m_view;
    if (!child)
        return;

    m_startDrag = mapToGlobal(child->pos());
    qDebug() << "startDrag:" << m_startDrag;

    QPoint hotSpot = pos - child->pos();
    QMimeData *mimeData = new QMimeData;
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << m_handler->pluginId() << m_handler->type() << hotSpot;
    mimeData->setData(EditModeMimeDataFormat, itemData);

    QPixmap pixmap(child->grab());
    pixmap.setMask(WidgetContainer::bitmapOfMask(pixmap.size(), WidgetHandlerImpl::get(m_handler)->m_isUserAreaInstance));

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(hotSpot);

    Qt::DropAction dropAction = drag->exec(Qt::CopyAction, Qt::CopyAction);
    if (dropAction == Qt::IgnoreAction) {
        gobackDrag(pixmap, QCursor::pos());
    }
}

void WidgetStoreCell::enterEvent(QEvent *event)
{
    Q_EMIT enterChanged(true);
    QWidget::enterEvent(event);
}

void WidgetStoreCell::leaveEvent(QEvent *event)
{
    Q_EMIT enterChanged(false);
    QWidget::leaveEvent(event);
}
WIDGETS_FRAME_END_NAMESPACE
