class Drawable
{
	public:
		Drawable();
		void Draw(SDL_Rect * quad, double x, double y, double rotation, double scalarX, double scalarY, SDL_Color color);
		~Drawable();

		int GetWidth();
		int GetHeight();

	protected:
		int width;
		int height;

		SDL_Texture * texture;
		SDL_Surface * surface;

		double scalarX;
};