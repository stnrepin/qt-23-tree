#ifndef GUI_TWO_THREE_TREE_CANVAS_HPP
#define GUI_TWO_THREE_TREE_CANVAS_HPP

#include <memory>
#include <queue>
#include <stack>

#include <QWidget>
#include <QPainter>

#include "2-3_tree_impl/2-3_tree.hpp"

// Include Qt generated files.
//
#include "gui/ui_two_three_tree_canvas.h"

namespace Ui {
    class TwoThreeTreeCanvas;
}

class TwoThreeTreeCanvas : public QWidget {
    Q_OBJECT

public:
    explicit TwoThreeTreeCanvas(QWidget* parent = nullptr);
    ~TwoThreeTreeCanvas() noexcept = default;

    void Clear();

    void PrintTree(const two_three_tree::TwoThreeTree& tree);

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    std::pair<float, float> CalculateNodePosition(int index, int depth, int tree_depth) const;

    std::unique_ptr<Ui::TwoThreeTreeCanvas> ui_;
    QPixmap pm_;
    const two_three_tree::TwoThreeTree* tree_;
};

#endif // !GUI_TWO_THREE_TREE_CANVAS_HPP

