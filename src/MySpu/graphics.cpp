#include "../../lib/TXLib.h"
#include "../../lib/mymathfunc/math_operation.h"

#include "spu_func.h"
#include "graphics.h"

void VideoRAM (const double *const ram) {

    txCreateWindow (WINDOW_X_SIZE, WINDOW_Y_SIZE);
    txSetDefaults ();

    SetBackground (TX_BLM);

    txSetColor (TX_GRAY, THICKNESSS);

    for (size_t i = 0; i < RAM_SIZE; i++) {

        IsDoubleEqual (ram[i], 0) ? txSetFillColor (TX_GRAY) : txSetFillColor (TX_GREEN);

        txCircle (X_OFFSET + (i % FIG_IN_ROW) * (2 * RADIUS_OF_CIRCLE + DIST_BEETWEEN_FIG),
                  Y_OFFSET + (i / FIG_IN_ROW) * (2 * RADIUS_OF_CIRCLE + DIST_BEETWEEN_FIG),
                  RADIUS_OF_CIRCLE);
    }
}

void SetBackground (const int tx_color) {

    txSetColor (tx_color, 0);

    txSetFillColor (tx_color);

    txRectangle (0, 0, WINDOW_X_SIZE, WINDOW_Y_SIZE);
}
     

