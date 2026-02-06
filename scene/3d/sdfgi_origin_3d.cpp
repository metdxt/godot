/**************************************************************************/
/*  sdfgi_origin_3d.cpp                                                   */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "sdfgi_origin_3d.h"

#include "servers/rendering/rendering_server.h"

void SDFGIOrigin3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_active", "active"), &SDFGIOrigin3D::set_active);
	ClassDB::bind_method(D_METHOD("is_active"), &SDFGIOrigin3D::is_active);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "active"), "set_active", "is_active");
}

void SDFGIOrigin3D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_WORLD: {
			if (active) {
				RID scenario = get_world_3d()->get_scenario();
				RenderingServer::get_singleton()->register_sdfgi_origin(scenario, get_instance_id());
			}
		} break;

		case NOTIFICATION_EXIT_WORLD: {
			if (is_inside_tree()) {
				RID scenario = get_world_3d()->get_scenario();
				RenderingServer::get_singleton()->unregister_sdfgi_origin(scenario, get_instance_id());
			}
		} break;

		case NOTIFICATION_TRANSFORM_CHANGED: {
			// Transform changes will automatically be picked up in the next render frame
		} break;
	}
}

void SDFGIOrigin3D::_update_active() {
	if (!is_inside_tree()) {
		return;
	}

	RID scenario = get_world_3d()->get_scenario();
	if (active) {
		RenderingServer::get_singleton()->register_sdfgi_origin(scenario, get_instance_id());
	} else {
		RenderingServer::get_singleton()->unregister_sdfgi_origin(scenario, get_instance_id());
	}
}

void SDFGIOrigin3D::set_active(bool p_active) {
	if (active != p_active) {
		active = p_active;
		_update_active();
	}
}

bool SDFGIOrigin3D::is_active() const {
	return active;
}

SDFGIOrigin3D::SDFGIOrigin3D() {
	set_notify_transform(true);
}