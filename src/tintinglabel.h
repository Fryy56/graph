#pragma once

#include <QLabel>
#include <QPropertyAnimation>

class TintingLabel final : public QLabel {
	Q_OBJECT

	Q_PROPERTY(QColor color READ getColor WRITE setColor)

public:
	explicit TintingLabel(QWidget*);
	~TintingLabel() override;

	QColor getColor() { return this -> palette().windowText().color(); }

	void setColor(QColor&);

private:
	QPropertyAnimation* m_toWhiteAnim;
	QPropertyAnimation* m_toGrayAnim;
};