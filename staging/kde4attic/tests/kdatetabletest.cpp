#include <QApplication>

#include <klocalizedstring.h>

#include "kdatetable.h"

int main( int argc, char** argv )
{
  QApplication::setApplicationName("test");
  QApplication app(argc, argv);

  KDateTable widget;
  widget.setCustomDatePainting( QDate::currentDate().addDays(-3), QColor("green"), KDateTable::CircleMode, QColor("yellow") );
  widget.show();

  return app.exec();
}