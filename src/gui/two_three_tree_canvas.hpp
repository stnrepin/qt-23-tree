#ifndef GUI_TWO_THREE_TREE_CANVAS_HPP
#define GUI_TWO_THREE_TREE_CANVAS_HPP

#include <memory>

#include <QWidget>
#include <QPainter>

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
    ~TwoThreeTreeCanvas() noexcept;

    template<typename Iter1, typename Iter2>
    void PrintTree(Iter1 begin, Iter2 end) {
        
    }

private:
    std::unique_ptr<Ui::TwoThreeTreeCanvas> ui_;
    QPainter* painter_;
};

#endif // !GUI_TWO_THREE_TREE_CANVAS_HPP

