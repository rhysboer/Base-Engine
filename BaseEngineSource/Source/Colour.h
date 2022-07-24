#pragma once
namespace BE {
	/// <summary>
	/// Stores colour in 0 to 255
	/// </summary>
	class Colour
	{
	public:
		Colour(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a);
		Colour(const float& r, const float& g, const float& b, const float& a);

		unsigned int Get255(const unsigned char& index);
		float Get01(const unsigned char& index);



	private:
		const unsigned char r, g, b, a;
	};
}

