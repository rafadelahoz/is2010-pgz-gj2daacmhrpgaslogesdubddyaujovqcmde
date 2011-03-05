#include "Image.h"

Image::Image(std::string path, GfxEngine* gfxEngine, bool transparent, bool write)
{
    // Almacenamos valores necesarios
    writeable = write;
    this->transparent = transparent;
    this->gfxEngine = gfxEngine;
    this->path = path;

	// Todav�a no se ha cargado nada
	loaded = false;
	wpic = NULL;
	rpic = NULL;

    // Cargamos la imagen en memoria
    loadImage(path, transparent);

}

Image::Image(int width, int height, GfxEngine* gfxEngine, bool transparent, bool write)
{
    // Almacenamos valores necesarios
    writeable = write;
    this->transparent = transparent;
    this->gfxEngine = gfxEngine;
    this->path = "";

	// No hemos cargado nada
	loaded = false;

    // Creamos la imagen con el tama�o dado
    wpic = gfxEngine->createImage(width, height);

    // Si la creaci�n de la imagen no ha fallado, asociamos la imagen creada
    // a la imagen que se va a pintar por pantalla
    if (wpic != NULL)
    {
        rpic = new sf::Sprite(wpic->GetImage());
    }
}

Image::~Image()
{
	// si hemos cargado alg�n archivo, lo liberamos
	if (loaded)
		gfxEngine->deleteImage(path);

    // borramos la imagen de s�lo lectura si no es vac�a
    if (rpic != NULL) delete rpic;

    // borramos la imagen modificable si no es vac�a
    if (wpic != NULL) delete wpic;
}

void Image::loadImage(std::string path, bool transparent)
{
	// Liberamos la imagen antigua si la hubiera
	if (loaded)
		gfxEngine->deleteImage(path);

	// Borramos las superficies si las tuvi�ramos
	if (rpic != NULL) delete rpic;
	if (wpic != NULL) delete wpic;

    // Cargamos la imagen gracias al motor gr�fico.
    rpic = new sf::Sprite(*gfxEngine->loadImage(path, transparent));

    // Si el proceso no ha fallado y la imagen es de escritura,
    // actualizamos el elemento renderizable.
    if (rpic != NULL)
    {
		loaded = true;

        if (writeable)
        {
            // Obtenemos las dimensiones de la imagen original
            sf::Vector2f size = rpic->GetSize();

            // Creamos el elemento sobreescribible
            wpic = new sf::RenderImage();

            // Lo inicializamos a una imagen vac�a de tama�o el de la imagen cargada
            // Si el proceso no falla, procedemos a actualizar la imagen de escritura
            if (wpic->Create((unsigned int) size.x, (unsigned int) size.y))
            {
                // Pintamos la imagen cargada sobre la imagen de escritura
                wpic->Draw(*rpic);

                // Actualizamos el pintado
                wpic->Display();
            }
        }
     }
}

void Image::refresh()
{
    // Actualizamos los cambios realizados sobre la imagen modificable
    wpic->Display();

    // Asignamos la imagen actual a la imagen a pintar (rpic)
    rpic->SetImage(wpic->GetImage());
}

int Image::getWidth()
{
    return (int) rpic->GetSize().x;
}

int Image::getHeigth()
{
    return (int) rpic->GetSize().y;
}

std::string Image::getPath()
{
    return path;
}

sf::RenderImage* Image::getSurfaceW()
{
    return wpic;
}

sf::Sprite* Image::getSurfaceR()
{
    return rpic;
}

