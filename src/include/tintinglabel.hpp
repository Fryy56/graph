#pragma once

#include <QLabel>
#include <QPropertyAnimation>


class TintingLabel : public QLabel {
	Q_OBJECT

	Q_PROPERTY(QColor color READ propGetColor WRITE propSetColor)

public:
	explicit TintingLabel(QWidget* parent = nullptr);
	explicit TintingLabel(QString const& text, QWidget* parent = nullptr);
	~TintingLabel() override;

	enum class ColorRoles {
		OffColor = 0,
		OnColor = 1,
	};
	static void setPresetColors(
		QColor const& offColor,
		QColor const& onColor
	);
	static void setPresetColors(QColor const& color, ColorRoles role);
	void setColors(
		QColor const& offColor = PresetColors.offColor,
		QColor const& onColor = PresetColors.onColor
	);
	void setColors(QColor const& color, ColorRoles role);

	void onSelect() { m_toOnAnim -> start(); }
	void onDeselect() { m_toOffAnim -> start(); }

protected:
	// Property getters/setters
	QColor propGetColor() const { return this -> palette().windowText().color(); }
	void propSetColor(QColor const&);

	// Fields
	inline static struct {
		QColor offColor;
		QColor onColor;
	} PresetColors;
	struct {
		QColor offColor;
		QColor onColor;
	} Colors;
public: // Funny getters
	static decltype(PresetColors) getPresetColors() { return PresetColors; }
	decltype(Colors) getColors() const { return Colors; }
protected:

	QPropertyAnimation* m_toOnAnim;
	QPropertyAnimation* m_toOffAnim;
};