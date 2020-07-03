#include "gui/two_three_tree_canvas.hpp"

#include <cmath>
#include <iostream>

TwoThreeTreeCanvas::TwoThreeTreeCanvas(QWidget* parent)
    : QWidget(parent), ui_(std::make_unique<Ui::TwoThreeTreeCanvas>()), tree_(nullptr)
{
    ui_->setupUi(this);
    pm_ = QPixmap(width()+300, height()+300);
}

void TwoThreeTreeCanvas::PrintTree(const two_three_tree::TwoThreeTree& tree) {
    QPainter painter(&pm_);

    if (tree.size() == 0) {
        return;
    }

    int tree_height = 0;
    std::queue<two_three_tree::TwoThreeTree::TwoThreeTreeNode*> q;
    two_three_tree::TwoThreeTree::TwoThreeTreeNode* node = tree.root()->left();
    q.push(node);
    while (true) {
        auto q_size = q.size();
        if (q_size == 0) {
            break;
        }

        ++tree_height; // depth

        while (q_size > 0) {
            node = q.front();
            q.pop();
            if (node->left() != nullptr) {
                q.push(node->left());
            }
            if (node->right() != nullptr) {
                q.push(node->right());
            }
            --q_size;
        }
    }

    std::stack<std::tuple<two_three_tree::TwoThreeTree::TwoThreeTreeNode*, int, int>> s;
    /*two_three_tree::TwoThreeTree::TwoThreeTreeNode**/ node = tree.root()->left();
    s.push({ node, 1, 0 });
    while (!s.empty()) {
        auto [node, index, depth] = s.top();
        s.pop();

        auto node_text = QString::number(node->key());
        auto [x, y] = CalculateNodePosition(index, depth, tree_height);
        painter.drawText(QRectF(x, y, 50, 50), node_text, QTextOption(Qt::AlignCenter));

        if (node->right() != nullptr) {
            s.push({ node->right(), index * 2 - 1, depth + 1 });
        }
        if (node->left() != nullptr) {
            s.push({ node->left(), index * 2, depth + 1 });
        }
    }
    update();
/*
    QPainter paint_node(this);
    std::vector<Tree::Data> nodes_data = _node->SetPositionOfNodesForVisualization();

        const int size = _node->DrawingSize/2;
        QPoint line_pos;
        line_pos.setX(_node->start_pos.x()+size);
        line_pos.setY(_node->start_pos.y()+size);
        std::vector<QPoint> nodes_lines = _node->GetVectorOfLinesBetwenNodes();
        for(const auto& a : nodes_lines){
            paint_node.drawLine(line_pos.x(),line_pos.y(),a.x()+size,a.y()+size);
            line_pos.setX(a.x()+size);
            line_pos.setY(a.y()+size);
        }

        paint_node.setBrush(QBrush(QColor(Qt::green)));
        for(const auto& a : nodes_data){
            QPoint node_pos(a.posX,a.posY);

            if(a.new_node == false){
                paint_node.drawEllipse(a.posX,a.posY,_node->DrawingSize,_node->DrawingSize);
            }
            else{
                paint_node.setBrush(QBrush(QColor(Qt::red)));
                paint_node.drawEllipse(a.posX,a.posY,_node->DrawingSize,_node->DrawingSize);
                paint_node.setBrush(QBrush(QColor(Qt::green)));
            }

            paint_node.drawText(QRectF(QPointF(node_pos),QSizeF(QSize(34,31))),QString::number(a.val),QTextOption(Qt::AlignCenter));
*/
}

void TwoThreeTreeCanvas::Clear() {
    QPainter painter(&pm_);
    painter.eraseRect(0, 0, pm_.width(), pm_.height());
    update();
}

void TwoThreeTreeCanvas::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, pm_.width(), pm_.height(), pm_);
}

std::pair<float, float> TwoThreeTreeCanvas::CalculateNodePosition(int index, int depth, int tree_depth) const {
    std::cout << index << " " << depth << " " << tree_depth << "\n";
    auto x = index * pm_.width() / (std::pow(2, depth) + 1);
    auto y = depth * pm_.height() / tree_depth;
    std::cout << x << " " << y << "\n";
    return { x, y };
}

