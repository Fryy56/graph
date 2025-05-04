#pragma once

#include <QLabel>
#include <QPropertyAnimation>


class TintingLabel final : public QLabel {
	Q_OBJECT

	Q_PROPERTY(QColor color READ getColor WRITE setColor)

	friend class HighlightedLineEdit;

public:
	explicit TintingLabel(QWidget* parent = nullptr);
	explicit TintingLabel(QString const& text, QWidget* parent = nullptr);
	~TintingLabel() override;

	static void setColors(
		QColor const&& offColor = std::move(Colors.offColor),
		QColor const&& onColor = std::move(Colors.onColor)
	);

private:
	// Getters/setters
	QColor getColor() { return this -> palette().windowText().color(); }
	void setColor(QColor const&);

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