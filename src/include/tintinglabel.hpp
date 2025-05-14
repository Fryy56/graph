#pragma once

#include <QLabel>
#include <QPropertyAnimation>


class TintingLabel : public QLabel {
	Q_OBJECT

	Q_PROPERTY(QColor color READ propGetColor WRITE propSetColor)

	friend class HighlightedLineEdit;

public:
	explicit TintingLabel(QWidget* parent = nullptr);
	explicit TintingLabel(QString const& text, QWidget* parent = nullptr);
	~TintingLabel() override;

	static void setColors(
		QColor const& offColor = Colors.offColor,
		QColor const& onColor = Colors.onColor
	);

protected:
	// Property getters/setters
	QColor propGetColor() const { return this -> palette().windowText().color(); }
	void propSetColor(QColor const&);

	void onSelect() { m_toOnAnim -> start(); }
	void onDeselect() { m_toOffAnim -> start(); }

	// Fields
	inline static struct {
		QColor offColor;
		QColor onColor;
	} Colors;

	QPropertyAnimation* m_toOnAnim;
	QPropertyAnimation* m_toOffAnim;
};