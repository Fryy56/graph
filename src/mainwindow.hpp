#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QLayout>
#include <QLineEdit>
#include <QRegularExpressionValidator>
#include "highlightedlineedit.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private:
	// Fields
	static struct {
		QColor BG;
		QColor Fields;
		QColor ButtonText;
		QColor GraphLine;
	} const Colors;

	QWidget* m_centralWidget;

	QHBoxLayout* m_mainLayout;
		QVBoxLayout* m_graphLayout;
			QWidget* m_graph;
			HighlightedLineEdit* m_function;
		QVBoxLayout* m_buttonLayout;
			QWidget* m_buttonBG;
				QVBoxLayout* m_optionsLayout;
					QLabel* m_buttonHeader;
					QHBoxLayout* m_limitsLayout;
						HighlightedLineEdit* m_limitMin;
						HighlightedLineEdit* m_limitMax;
			QPushButton* m_resetFunctionButton;
	
	QRegularExpressionValidator* m_intRegExVal;
};