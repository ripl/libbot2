/*
 * This file is part of bot2-vis.
 *
 * bot2-vis is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * bot2-vis is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with bot2-vis. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BOT2_VIS_BOT_VIS_SCROLLPLOT2D_H_
#define BOT2_VIS_BOT_VIS_SCROLLPLOT2D_H_

/**
 * @defgroup BotGlScrollPlot2d Plotting time-varying signals
 * @brief Plotting windows of time-varying signals
 * @ingroup BotVisGl
 * @include: bot_vis/bot_vis.h
 *
 * BotGlScrollPlot2d provides a way to plot time-varying signals in an OpenGL
 * context.  Data points are individually added as (x, y) pairs, where it is
 * expected that the x values increase over time.  Multiple signals can be
 * overlaid on the same window.
 *
 * Plots can be rendered anywhere in the OpenGL window, and have a number of
 * configurable options (e.g., axis limits, title, axis labels, legends,
 * background color, border color, and line color).
 *
 * Linking: `pkg-config --libs bot2-vis`
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Values to determine where to draw the legend for a BotGlScrollPlot2d
 */
typedef enum {
    /** do not draw the legend **/
    BOT_GL_SCROLLPLOT2D_HIDDEN,
    /** draw the legend at the top left corner of the plotting window **/
    BOT_GL_SCROLLPLOT2D_TOP_LEFT,
    /** draw the legend at the top right corner of the plotting window **/
    BOT_GL_SCROLLPLOT2D_TOP_RIGHT,
    /** draw the legend at the bottom left corner of the plotting window **/
    BOT_GL_SCROLLPLOT2D_BOTTOM_LEFT,
    /** draw the legend at the bottom right corner of the plotting window **/
    BOT_GL_SCROLLPLOT2D_BOTTOM_RIGHT
} BotGlScrollPlot2dLegendLocation;

typedef struct _BotGlScrollPlot2d BotGlScrollPlot2d;

/**
 * Constructor.
 */
BotGlScrollPlot2d * bot_gl_scrollplot2d_new (void);

/**
 * Destructor.
 */
void bot_gl_scrollplot2d_free (BotGlScrollPlot2d *self);

/**
 * Sets the plot title.  The title is drawn in the text color set by
 * bot_gl_scrollplot2d_set_text_color(), and can be hidden or shown with
 * bot_gl_scrollplot2d_set_show_title().
 */
int bot_gl_scrollplot2d_set_title (BotGlScrollPlot2d *self, const char *title);

/**
 * Sets the color of drawn text.
 * @param r red
 * @param g green
 * @param b blue
 * @param a alpha
 *
 * Color values should be in the range [0, 1]
 */
int bot_gl_scrollplot2d_set_text_color (BotGlScrollPlot2d *self,
        double r, double g, double b, double a);

/**
 * Sets whether or not to show the title.
 */
void bot_gl_scrollplot2d_set_show_title (BotGlScrollPlot2d *self, int val);

//void bot_gl_scrollplot2d_set_show_ylim (BotGlScrollPlot2d *self, int val);

/**
 * Sets the plot background color.
 * @param r red
 * @param g green
 * @param b blue
 * @param a alpha
 *
 * Color values should be in the range [0, 1]
 */
int bot_gl_scrollplot2d_set_bgcolor (BotGlScrollPlot2d *self,
        double r, double g, double b, double alpha);

/**
 * Sets the plot border color.  The border is drawn as a thin solid line.
 * @param r red
 * @param g green
 * @param b blue
 * @param a alpha
 *
 * Color values should be in the range [0, 1]
 */
int bot_gl_scrollplot2d_set_border_color (BotGlScrollPlot2d *self,
        double r, double g, double b, double alpha);

/**
 * Sets if and where to draw the plot legend.
 */
int bot_gl_scrollplot2d_set_show_legend (BotGlScrollPlot2d *self,
        BotGlScrollPlot2dLegendLocation where);

/**
 * Sets the x-axis limits.
 */
int bot_gl_scrollplot2d_set_xlim (BotGlScrollPlot2d *self, double xmin,
        double xmax);

/**
 * Sets the y-axis limits.
 */
int bot_gl_scrollplot2d_set_ylim (BotGlScrollPlot2d *self, double ymin,
        double ymax);

/**
 * Adds a new signal to the plotting window.
 * @param name name of the signal.  Must be different from other signals names in
 * this instance.
 * @param max_points the maximum number of data points to store in memory.  Defaults
 * to 1000.
 */
int bot_gl_scrollplot2d_add_plot (BotGlScrollPlot2d *self, const char *name,
        int max_points);

/**
 * Removes a signal from the plotting window
 * @param name name of the signal to remove.
 */
int bot_gl_scrollplot2d_remove_plot (BotGlScrollPlot2d *self, const char *name);

/**
 * Sets the color that a signal is drawn in.
 * @param name name of the specified signal
 * @param r red
 * @param g green
 * @param b blue
 * @param a alpha
 *
 * Color values should be in the range [0, 1]
 */
int bot_gl_scrollplot2d_set_color (BotGlScrollPlot2d *self, const char *name,
        double r, double g, double b, double alpha);

/**
 * Adds an (x,y) data point to a signal
 * @param name name of the signal
 */
int bot_gl_scrollplot2d_add_point (BotGlScrollPlot2d *self, const char *name,
        double x, double y);

/**
 * Renders plots using the current OpenGL context at the specified location and
 * size in window coordinates, where (0,0) is the top left of the window.
 */
void bot_gl_scrollplot2d_gl_render_at_window_pos (BotGlScrollPlot2d *self,
        int topleft_x, int topleft_y, int width, int height);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif  /* BOT2_VIS_BOT_VIS_SCROLLPLOT2D_H_ */
