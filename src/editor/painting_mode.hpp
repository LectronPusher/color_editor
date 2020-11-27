#pragma once

namespace editor {
namespace painting_mode {

/* 
 * this is where I'm putting the painting_mode mode enum,
 * anywhere else is weird, and has dumb header recompilings if I muck with it, 
 * this is simple and easy to explicitly include if being used
 * 
 * currently used in model_renderer and effect
 */

enum mode {replace, over};
const mode default_mode = replace;

} // painting_mode
} // editor
