/* ipl - Image Processing Library  
// See LICENSE file for copyright and license details. 
*/
void printInfo(Pics *img);
void printResInfo(Dime *dime);
void printMenu();
void printResMenu();
char* calcSize(const char* result);
void resize(Pics *img, Dime *dime);
void getName(Dime *dime);
void getWidth(Dime *dime);
void getHeight(Dime *dime);
unsigned char* performResize(unsigned char* imageData, int width, int height, int channel, int newWidth, int newHeight);
Dime resizeBack(Dime *dime);
Dime resizeHero(Dime *dime);
Dime resizeBanner(Dime *dime);
Dime resizeBlog(Dime *dime);
Dime resizeLogoRec(Dime *dime);
Dime resizeLogoSc(Dime *dime);
Dime resizeFavicon(Dime *dime);
Dime resizeSocial(Dime *dime);
Dime resizeLight(Dime *dime);
Dime resizeThumb(Dime *dime);
Dime resizeCustom(Dime *dime);
int back(int *backToMainMenu);
void crop();
void rotate();
void saveImage(unsigned char* imageData, int width, int height, int channel, const char* filename);
void quit();
void clearInputBuffer();
