#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QLayout>
#include <QLineEdit>
#include <QRegularExpressionValidator>
#include <QSettings>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include "graph.hpp"
#include "include/highlightedlineedit.hpp"
#include "include/separator.hpp"
#include "include/colorpicker.hpp"


class Graph;
class GraphView;

class MainWindow : public QMainWindow {
	Q_OBJECT

	friend class Graph;

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
		QColor ButtonBorder;
		QColor LabelText;
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

	// Objecttree (m_centralWidget)
		QHBoxLayout* m_mainLayout;
			QVBoxLayout* m_graphLayout;
				GraphView* m_graphView;
					QGraphicsScene* m_graphScene;
						QGraphicsProxyWidget* m_graphProxy;
							Graph* m_graph;
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
						QLabel* m_graphColorLabel;
						ColorPicker* m_graphColorPicker;
				QPushButton* m_plotGraphButton;
				QPushButton* m_resetFunctionButton;
	// Objecttree (m_centralWidget)
	
	QRegularExpressionValidator* m_intRegExVal;
	QRegularExpressionValidator* m_functionRegExVal;

private slots:
	void onPlot();
};