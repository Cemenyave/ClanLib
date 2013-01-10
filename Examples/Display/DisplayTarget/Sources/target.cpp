/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Mark Page
*/

#include "precomp.h"
#include "target.h"
#include "framerate_counter.h"

Target::Target()
{
	render_target = d3d;
}

int Target::start(const std::vector<std::string> &args)
{
	clan::SetupGL target_opengl2;
	clan::SetupGL1 target_opengl1;
	clan::SetupD3D target_d3d;

	// Since OpenGL 1.3 is compatible and fast - Use that as the default
	render_target = opengl1;

	do
	{
		clan::KeepAlive::process(0);	// <-- This is important, to flush the win32 keyboard queue

		switch (render_target)
		{
			case (opengl1):
				target_opengl1.set_current();
				break;

			case (opengl2):
				target_opengl2.set_current();
				break;

			case (d3d):
				target_d3d.set_current();
				break;
		}

	}while (run_demo());

	return 0;
}

bool Target::run_demo()
{
	clan::DisplayWindowDescription desc;
	desc.set_title("Target Display (with resizable window)");
	desc.set_size(clan::Size(640, 480), true);
	desc.set_allow_resize(true);
	clan::DisplayWindow window(desc);

	// Connect the Window close event
	clan::Slot slot_quit = window.sig_window_close().connect(this, &Target::on_window_close);

	// Connect a keyboard handler to on_key_up()
	clan::Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &Target::on_input_up);

	clan::Canvas canvas(window);

	clan::Font target_font(canvas, "tahoma", 32);
	clan::Font fps_font(canvas, "tahoma", 20);

	// Because we are using the alpha channel on fonts, we must disable subpixel rendering
	clan::FontDescription font_desc;
	font_desc.set_typeface_name("tahoma");
	font_desc.set_height(16);
	font_desc.set_subpixel(false);
	clan::Font fall_font(canvas, font_desc);

	std::vector<FontFall> font_fall;

	FramerateCounter frameratecounter;

	unsigned int time_last = clan::System::get_time();
	float fontfall_ticker = 0.0f;

	quit = false;
	// Run until someone presses escape
	while (true)
	{
		if (window.get_ic().get_keyboard().get_keycode(clan::keycode_1))
		{
			render_target = opengl1;
			break;
		}
		if (window.get_ic().get_keyboard().get_keycode(clan::keycode_2))
		{
			render_target = opengl2;
			break;
		}
		if (window.get_ic().get_keyboard().get_keycode(clan::keycode_3))
		{
			render_target = d3d;
			break;
		}
		if (window.get_ic().get_keyboard().get_keycode(clan::keycode_escape))
		{
			quit = true;
			break;
		}

		unsigned int time_now = clan::System::get_time();
		float time_delta = (float) (time_now - time_last);
		time_last = time_now;

		fontfall_ticker += time_delta;
		if (fontfall_ticker >= 10.0f)
		{
			fontfall_ticker = 0.0f;
			font_fall.push_back(new_fontfall(canvas.get_width()));
		}

		canvas.clear(clan::Colorf(0.0f,0.0f,0.0f));

		std::string fps = clan::string_format("%1 fps", frameratecounter.get_framerate());
		fps_font.draw_text(canvas, canvas.get_width() - 100, 30, fps);

		std::string words = clan::string_format("%1 words", (int) font_fall.size());
		fps_font.draw_text(canvas, 100, 30, words);

		const int font_xpos = 32;
		const int font_ypos = 64;
		switch (render_target)
		{
			case (opengl1):
				target_font.draw_text(canvas, font_xpos, font_ypos, "1) OpenGL 1.3 (clanGL1)");
				break;

			case (opengl2):
				target_font.draw_text(canvas, font_xpos, font_ypos, "2) OpenGL 3.2 or higher(clanGL)");
				break;

			case (d3d):
				target_font.draw_text(canvas, font_xpos, font_ypos, "3) Direct3D renderer (clanD3D)");
				break;
		}
		fps_font.draw_text(canvas, 32, 96, "Press 1,2 or 3 to select targets, or escape to quit.");

		float max_height = (float) (canvas.get_height() + 20);
		float half_height = (float) canvas.get_height() / 2.0f;
		for (std::vector<FontFall>::iterator it = font_fall.begin(); it != font_fall.end();)
		{
			FontFall &item = *it;
			item.ypos += time_delta / 10.0f;
			if (item.ypos >= max_height)
			{
				it = font_fall.erase(it);
			}
			else
			{
				item.color.a = item.ypos / (half_height);
				if (item.color.a > 1.0f)
					item.color.a = 1.0f;
				fall_font.draw_text(canvas, item.xpos, (int) item.ypos, item.text, item.color);
				++it;
			}
		}

		canvas.flip(0);
		frameratecounter.frame_shown();

		clan::KeepAlive::process(0);
	}
	return !quit;
}

FontFall Target::new_fontfall(int window_width)
{
	static std::string words[] = 
	{
		"Apple","Ball","ClanLib","Dog","Egg","Fish","Ghost","Hunter",
		"Ice cream","Jumper","Kite","Lion","Matrix","Nose","Orange","Panda",
		"Queen","Ruler","Screen saver","Trousers","Umbrella","Violin","Window","Xylophone",
		"Yacht","Zebra"
	};

	static int word_counter = 0;

	FontFall fontfall;
	// (Note, do not randomize the word order, because it would will make comparing display target fps more difficult)
	fontfall.text = words[++word_counter % (sizeof(words)/sizeof(std::string))];
	unsigned int value = rand();
	fontfall.xpos = (value % window_width) - 10;
	fontfall.ypos = 0.0f;
	fontfall.color.r = (rand() & 255) / 255.0f;
	fontfall.color.g = (rand() & 255) / 255.0f;
	fontfall.color.b = (rand() & 255) / 255.0f;
	fontfall.color.a = 1.0f;
	return fontfall;
}

// A key was pressed
void Target::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void Target::on_window_close()
{
	quit = true;
}


