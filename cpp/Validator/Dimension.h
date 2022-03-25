#pragma once

namespace validator {
	class Dimension {
	public:
		Dimension(const unsigned int l, const unsigned int w, const unsigned int h) : l(l), w(w), h(h), volume(l*w*h) {}
		Dimension() : l(0), w(0), h(0), volume(0) {}
		unsigned int l;
		unsigned int w;
		unsigned int h;
		unsigned int volume;
	};
}