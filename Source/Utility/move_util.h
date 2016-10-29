#pragma once

// will declare standart move constructor + delete copy constuctor
#define MOVE_ONLY(name) name(const name&) = delete; name& operator=(const name&) = delete; \
						name(name&&) = default; name& operator=(name&&) = default