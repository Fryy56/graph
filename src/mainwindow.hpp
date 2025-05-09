#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QLayout>
#include <QLineEdit>
#include <QRegularExpressionValidator>
#include <QSettings>
#include "include/highlightedlineedit.hpp"
#include "include/separator.hpp"

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
	void readSettings();
	void writeSettings();

	// Fields
	static struct {
		QColor BG;
		QColor Fields;
		QColor ButtonText;
	} const Colors;
	static struct {
		// win/
			QSize windowSize;
			bool isMax;
		// fields/
			QString function;
			QString minX;
			QString maxX;
		QColor graphLineColor;
	} const DefaultValues;
	struct {
		// win/
		QSize windowSize;
		bool isMax;
		// fields/
			QString function;
			QString minX;
			QString maxX;
		QColor graphLineColor;
	} SettingsValues;

	QSettings* m_settings;

	QWidget* m_centralWidget;

		// Objecttree
	QHBoxLayout* m_mainLayout;
		QVBoxLayout* m_graphLayout;
			QWidget* m_graph;
			HighlightedLineEdit* m_function;
		QVBoxLayout* m_buttonLayout;
			QWidget* m_buttonBG;
				QVBoxLayout* m_optionsLayout;
					QLabel* m_optionsHeader;
					Separator* m_optionsSeparator;
					QHBoxLayout* m_limitsLayout;
						HighlightedLineEdit* m_limitMin;
						QVBoxLayout* m_resetLimitsButtonOffsetLayout;
							QPushButton* m_resetLimitsButton;
						HighlightedLineEdit* m_limitMax;
			QPushButton* m_resetFunctionButton;
		// Objecttree
	
	QRegularExpressionValidator* m_intRegExVal;
};