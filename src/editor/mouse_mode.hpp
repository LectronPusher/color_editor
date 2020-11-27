#pragma once

namespace editor {
namespace mouse_mode {

/* 
 * this is where I'm putting the mouse_mode mode enum,
 * anywhere else is weird, and has dumb header recompilings if I muck with it, 
 * this is simple and easy to explicitly include if being used
 */

// -1 to work with QButtonGroup
enum mode {none = -1, pan, combined_points, single_point};

} // mouse_mode
} // editor
