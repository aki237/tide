#include <Color.h>

int main(int argc, char *argv[]) {
    const std::string validRGB = "#34F";
    const std::string validRRGGBB = "#344556";
    const std::string validRRGGBBAA = "#344556FF";
    const std::string invalid = "344556FF";
    Color vRGB(validRGB);// return invalid character
    vRGB.Print();
    Color vRRGGBB(validRRGGBB);// return valid color with format #RRGGBB
    vRRGGBB.Print();
    Color vRRGGBBAA(validRRGGBBAA);// return valid color with format #RRGGBBAA
    vRRGGBBAA.Print();
    Color iv(invalid);// return invalid color string
    iv.Print();
    Color rgbaToHTML(52, 69, 86, 255);
    std::cout << rgbaToHTML.ToHTMLColor() << std::endl; // return #344556FF
    return 0;
}
