#include "gui/two_three_tree_canvas.hpp"

#include <cmath>

#include <QBrush>
#include <QFont>
#include <QRectF>
#include <QPen>
#include <QPointF>
#include <QString>
#include <QTextOption>

TwoThreeTreeCanvas::TwoThreeTreeCanvas(const two_three_tree::TwoThreeTree* tree,
                                       const TwoThreeTreeCanvasOptions& opt,
                                       QWidget* parent)
    : QWidget(parent),
      ui_(std::make_unique<Ui::TwoThreeTreeCanvas>()),
      pm_(width(), height()),
      opt_(opt),
      tree_(tree)
{
    ui_->setupUi(this);
}

void TwoThreeTreeCanvas::PrintTree() {
    if (tree_ == nullptr || tree_->size() == 0) {
        Clear();
        return;
    }

    QPainter painter(&pm_);
    painter.setFont(QFont("times", opt_.node_key_font_size));
    painter.setPen(QPen(Qt::black, opt_.pen_size));
    painter.setBrush(QBrush(Qt::white));

    auto tree_depth = tree_->GetDepth();

    auto callback = [=, &painter](auto key, auto index, auto depth, auto is_left) {
        auto node_text = QString::number(key);
        auto [x, y] = CalculateNodePosition(index, depth, tree_depth);

        if (!(index == 1 && depth == 0)) {
            auto parent_index = is_left ? (index + 1) / 2 : index / 2;
            auto [parent_x, parent_y] = CalculateNodePosition(
                                            parent_index, depth - 1, tree_depth);
            painter.drawLine(x, y, parent_x, parent_y);
        }

        auto r = opt_.node_ellipse_radius;
        painter.drawEllipse(QPointF(x, y), r, r);

        auto d = r * 2;
        painter.drawText(QRectF(x-r, y-r, d, d), node_text,
                            QTextOption(Qt::AlignCenter));
    };
    tree_->DoPostOrderTraversal(callback);
    update();
}

void TwoThreeTreeCanvas::Clear() {
    pm_.fill(Qt::white);
    update();
}

void TwoThreeTreeCanvas::resizeEvent(QResizeEvent* e) {
    pm_ = QPixmap(e->size().width(), e->size().height());
    PrintTree();
}

void TwoThreeTreeCanvas::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, pm_.width(), pm_.height(), pm_);
}

std::pair<float, float> TwoThreeTreeCanvas::CalculateNodePosition(
        int index, int depth, int tree_depth) const
{
    auto x = index * pm_.width() / (std::pow(2, depth) + 1);
    auto y = depth * pm_.height() / tree_depth + opt_.canvas_padding;
    return { x, y };
}

