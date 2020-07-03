#include "gui/two_three_tree_canvas.hpp"

#include <cmath>

#include <QBrush>
#include <QFont>
#include <QRectF>
#include <QPen>
#include <QPointF>
#include <QString>
#include <QTextOption>

TwoThreeTreeCanvas::TwoThreeTreeCanvas(QWidget* parent)
    : QWidget(parent), ui_(std::make_unique<Ui::TwoThreeTreeCanvas>()), tree_(nullptr)
{
    ui_->setupUi(this);
    pm_ = QPixmap(width(), height());
}

void TwoThreeTreeCanvas::PrintTree() {
    PrintTree(tree_);
}

void TwoThreeTreeCanvas::PrintTree(const two_three_tree::TwoThreeTree* tree) {
    tree_ = tree;
    if (tree_ == nullptr || tree_->size() == 0) {
        Clear();
        return;
    }

    QPainter painter(&pm_);
    painter.setFont(QFont("times", 16));
    painter.setPen(QPen(Qt::black, 4));
    painter.setBrush(QBrush(Qt::white));

    auto tree_depth = tree_->GetDepth();

    // Node, Index, Depth, IsLeft
    std::stack<
        std::tuple<
            two_three_tree::TwoThreeTree::TwoThreeTreeNode*, int, int, bool
        >
    > s;
    auto* node = tree_->root()->left();
    auto* head = node;
    s.push({ node, 1, 0, false });
    while (!s.empty()) {
        auto [node, index, depth, is_left] = s.top();

        auto is_finish_subtree = (node->right() == head || node->left() == head);
        auto is_leaf = (node->left() == nullptr && node->right() == nullptr);
        if (is_finish_subtree || is_leaf) {
            s.pop();
            auto node_text = QString::number(node->key());
            auto [x, y] = CalculateNodePosition(index, depth, tree_depth);

            if (!(index == 1 && depth == 0)) {
                auto parent_index = is_left ? (index + 1) / 2 : index / 2;
                auto [parent_x, parent_y] = CalculateNodePosition(
                                                parent_index, depth - 1, tree_depth);
                painter.drawLine(x, y, parent_x, parent_y);
            }

            painter.drawEllipse(QPointF(x, y), 20, 20);

            painter.drawText(QRectF(x-20, y-20, 40, 40), node_text,
                             QTextOption(Qt::AlignCenter));
            head = node;
        }
        else {
        if (node->right() != nullptr) {
            s.push({ node->right(), index * 2, depth + 1, false });
        }
        if (node->left() != nullptr) {
            s.push({ node->left(), index * 2 - 1, depth + 1, true });
        }
    }
  }
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
    auto y = depth * pm_.height() / tree_depth + 40;
    return { x, y };
}

