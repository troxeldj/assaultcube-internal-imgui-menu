#include "openglDraw.h"
#include "hack.h"

class GL::Font f;

void GL::SetupOrtho()
{
	// Pushes all statevariables onto attribute stack.
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	// glPushMatrix pushes the current matrix stack down by one, duplicating the current matrix. 
	// That is, after a glPushMatrix call, the matrix on top of the stack is identical to the one below it.
	glPushMatrix();
	// Create an array of four ints for viewport
	// Viewport: Rectangle in pixels on screen that you wish to render to.
	GLint viewport[4];
	// Returns 4 values:
	// 1: x coordinates of window
	// 2: y coordinates of window
	// 3: Width
	// 4: Height
	glGetIntegerv(GL_VIEWPORT, viewport);
	// Sets the viewport x,y to zero and width and height
	glViewport(0, 0, viewport[2], viewport[3]);
	// Specifies that we are working with the projection matrix and matrix stack.
	glMatrixMode(GL_PROJECTION);
	// Replace the current Matrix with identity matrix
	glLoadIdentity();
	// Multiplies the current matrix with orgographic matrix
	glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
	// Specifies that we are working with the projection matrix and matrix stack.
	glMatrixMode(GL_MODELVIEW);
	// Replaces current matrix with identity matrix
	glLoadIdentity();
	// Don't update the depth buffer
	glDisable(GL_DEPTH_TEST);
}

void GL::RestoreGL()
{
	// Pops the current matrix stack, replacing the current matrix with the one below it on the stack
	glPopMatrix();
	// restores the values of the state variables saved with the last glPushAttrib (above)
	glPopAttrib();
}

void GL::DrawFilledRect(float x, float y, float width, float height, ImVec4 color)
{
	// Sets the current color using rgb values
	glColor4f(color.x, color.y, color.z, color.w);
	// We're beginning to draw
	// GL_QUADS = Treats each group of four vertices as an independent quadrilateral
	glBegin(GL_QUADS);
	// top left vertex
	glVertex2f(x, y);
	// top right vertex
	glVertex2f(x + width, y);
	// bottom right vertex
	glVertex2f(x + width, y + height);
	// bottom left vertex
	glVertex2f(x, y + height);
	// end our drawing
	glEnd();
}

void GL::DrawOutline(float x, float y, float x2, float y2, float lineWidth, ImVec4 color)
{
	glLineWidth(lineWidth);
	glColor4f(color.x, color.y, color.z, color.w);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x, y);
	glVertex2f(x2, y);
	glVertex2f(x2, y2);
	glVertex2f(x, y2);
	glEnd();
}

void GL::DrawLine(float startx, float starty, float endx, float endy, float lineWidth, ImVec4 color) {
	glColor4f(color.x, color.y, color.z, color.w);
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	glVertex2f(startx, starty);
	glVertex2f(endx, endy);
	glEnd();
}

float GL::getScreenx() {

	float viewport[4] = { 0 };
	glGetFloatv(GL_VIEWPORT, viewport);

	return viewport[2];
}
float GL::getScreenY() {
	float viewport[4] = { 0 };
	glGetFloatv(GL_VIEWPORT, viewport);

	return viewport[3];
}

void GL::Font::Build(int height) {
	hdc = wglGetCurrentDC();
	base = glGenLists(96);
	HFONT hFont = CreateFontA(-height, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Arial");
	HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
	
	wglUseFontBitmaps(hdc, 32, 96, base);
	SelectObject(hdc, hOldFont);
	DeleteObject(hFont);

	bBuilt = true;
}
void GL::Font::Print(float x, float y, ImVec4 color, const char* format, ...)
{
	HDC fontHDC = wglGetCurrentDC();

	if (fontHDC != f.hdc)
	{
		base = glGenLists(96);
		HFONT hFont = CreateFontA(-hack::fontHeight, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Arial");
		HFONT hOldFont = (HFONT)SelectObject(fontHDC, hFont);
		wglUseFontBitmaps(fontHDC, 32, 96, base);
		SelectObject(fontHDC, hOldFont);
		DeleteObject(hFont);
		f.hdc = fontHDC;
	}

	glColor4f(color.x, color.y, color.z, color.w);
	glRasterPos2f(x, y);

	char text[100];
	va_list	args;

	va_start(args, format);
	vsprintf_s(text, 100, format, args);
	va_end(args);

	glPushAttrib(GL_LIST_BIT);
	glListBase(base - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}

Vector3 GL::Font::centerText(float x, float y, float width, float height, float textWidth, float textHeight) {
		Vector3 text;
		text.x = x + (width - textWidth) / 2;
		text.y = y + textHeight;
		return text;
}

float GL::Font::centerText(float x, float width, float textWidth) {
	if (width > textWidth)
	{
		float difference = width - textWidth;
		return (x + (difference / 2));
	}
	else
	{
		float difference = textWidth - width;
		return(x - (difference / 2));
	}
}