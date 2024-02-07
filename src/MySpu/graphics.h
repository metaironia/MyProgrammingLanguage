#ifndef GRAPHICS_H
#define GRAPHICS_H

const double THICKNESSS = 4;

const int RADIUS_OF_CIRCLE = 10;

const int FIG_IN_ROW = 30;

const int DIST_BEETWEEN_FIG = RADIUS_OF_CIRCLE / 3;

const int X_OFFSET = RADIUS_OF_CIRCLE + DIST_BEETWEEN_FIG;
const int Y_OFFSET = RADIUS_OF_CIRCLE + DIST_BEETWEEN_FIG;

const int WINDOW_X_SIZE = FIG_IN_ROW * (2 * RADIUS_OF_CIRCLE + DIST_BEETWEEN_FIG) + DIST_BEETWEEN_FIG;
const int WINDOW_Y_SIZE = WINDOW_X_SIZE;

void VideoRAM (const double *ram);

void SetBackground (const int tx_color);

#endif
