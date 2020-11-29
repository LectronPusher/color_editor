#pragma once

#include "effect_state.hpp"

#include <QImage>
#include <QRect>
#include <QPainter>

namespace editor {
namespace color {

/* This is the function that creates a color effect
 * it takes as input the source image, the boundary of the selected region and
 * an effect_state
 * from the effect_state's state_type it delegates to one of the below painting
 * functions, that each have their own relevant inputs
 * 
 * this function can be used anywhere that an effect_state and a source image are
 * useful
 * 
 * note, the effect will be the same size as the source image, and will be 
 * altered outside of the boundary rect, the boundary rect is only currently 
 * used for setting the edges of the gradient
 * 
 * TODO make a GUI element that holds little effect_states and has small paintings
 * of them, (paintings would use this function) the user could select an effect
 * and color_editor would switch to that state
 */
QImage create_effect(const effect_state &state, const QImage &image,
					 const QRect &boundary);

/* paint_single_color just fills the entire effect with the stored color in s_c
 * literally: effect.fill(s_c.stored_color);
 */
void paint_single_color(QImage &effect, single_color_state s_c);

/* paints the gradient from the gradient_state
 * the edges of the gradient will be the edges of the boundary rect, but it will
 * be painted on the entire image
 */
void paint_gradient(QImage &effect, gradient_state g, const QRect &boundary);

/* pixellates the image from the top left, the bottom and right edges will have
 * pixels that are cut off if the pixel size doesn't divide the image exactly
 * 
 * does the pixellation by taking the sourcecolor at the center of a pixel
 * location and expanding it to fill the pixel as one color
 */
void paint_pixellate(QImage &effect, pixellate_state p, const QImage &source);

}; // color
}; // editor
