#ifndef UI_RENDERAREA_H_
#define UI_RENDERAREA_H_

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include "../logic/Universe.h"

class RenderArea : public QWidget {

public:
	RenderArea(QWidget *parent = 0, Universe *universe = 0);
	virtual ~RenderArea();

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	Universe *universe;

};

#endif /* UI_RENDERAREA_H_ */
