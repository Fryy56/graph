#pragma once

#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QMenu>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QWidgetAction>
#include "colordisplay.hpp"
#include "highlightedlineedit.hpp"


class ColorPickerWidget;

class ColorPicker : public ColorDisplay {
	Q_OBJECT

public:
	explicit ColorPicker(
		QColor& displayedColor,
		QColor const& borderColor = Qt::black,
		int borderWidth = 3,
		int borderRadius = 10,
		QWidget* parent = nullptr
	);
	~ColorPicker() override;

	// Getters
	QLabel* getEditLabel() const { return m_editLabel; }

protected:
	// Events
	void mousePressEvent(QMouseEvent*) override;
	void enterEvent(QEnterEvent*) override { editLabelFadeIn(); }
	void leaveEvent(QEvent*) override { editLabelFadeOut(); }

	// Animation functions
	void editLabelFadeIn();
	void editLabelFadeOut();

	// Fields
	QHBoxLayout* m_editLayout;
	
	// Objecttree (m_editLayout)
		QLabel* m_editLabel;
	// Objecttree (m_editLayout)

	QGraphicsOpacityEffect* m_opacityEffect;
	QPropertyAnimation* m_editLabelOpacityAnim;
	QMenu* m_colorPickerMenu;

	ColorPickerWidget* m_colorPickerWidget;
	QWidgetAction* m_colorPickerWidgetAction;
};

// ---------------------------------------------------------------------------

class ColorPickerWidget : public QWidget {
	Q_OBJECT

public:
	explicit ColorPickerWidget(QWidget* parent = nullptr);
	~ColorPickerWidget() override;

protected:
	// Fields
	QHBoxLayout* m_mainLayout;

	// Objecttree (m_mainLayout)
		QGridLayout* m_rgbHsvLayout;
	// Objecttree (m_mainLayout)
};