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

#ifndef BOT2_VIS_BOT_VIS_CONSOLE_H_
#define BOT2_VIS_BOT_VIS_CONSOLE_H_

/**
 * @defgroup console Vertically scrolling text overlay
 * @brief Vertically scrolling text
 * @ingroup BotVisGl
 * @include: bot_vis/bot_vis.h
 *
 * Utility class for rendering vertically scrolling text.  Text can be added
 * with printf-style formatting, and will scroll vertically as more text is
 * added.  A decay value can also be set to fade away text over time.
 *
 * This class can be useful for displaying textual debugging or status
 * information in an OpenGL window.
 *
 * Linking: `pkg-config --libs bot2-vis`
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _BotGlConsole BotGlConsole;

/**
 * Constructor.
 */
BotGlConsole *bot_gl_console_new(void);

/**
 * Destructor.
 */
void bot_gl_console_destroy(BotGlConsole *console);

/**
 * Sets the rendering font.  See GLUT documentation for possible options.
 */
void bot_gl_console_set_glut_font(BotGlConsole *console, void *font);

/**
 * Sets how quickly text will fade away.
 */
void bot_gl_console_set_decay_lambda(BotGlConsole *console, double lambda);

/**
 * Sets the text color.
 */
void bot_gl_console_color3f(BotGlConsole *console, float r, float g, float b);

/**
 * Adds text to display.  Uses printf-style formatting.
 */
void bot_gl_console_printf(BotGlConsole *console, const char *format, ...);

/**
 * Render the scrolling text.
 * @param elapsed_time how much time, in seconds, has elapsed since the last
 * rendering.  This is used in combination with
 * bot_gl_console_set_decay_lambda() to determine the text transparency.  If
 * less than zero, then the elapsed time will be automatically determined using
 * the system clock.
 */
void bot_gl_console_render(BotGlConsole *console, double elapsed_time);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif  /* BOT2_VIS_BOT_VIS_CONSOLE_H_ */
