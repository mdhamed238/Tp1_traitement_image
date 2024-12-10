#include <opencv2/opencv.hpp>
#include <vector>

cv::Mat applyConvolution(const cv::Mat &img, const std::vector<std::vector<float>> &noyau)
{
    // Get dimensions for easier access
    const int noyauSize = noyau.size();
    const int padding = noyauSize / 2;

    // Créer une image vide pour stocker le résultat
    cv::Mat filtered = cv::Mat::zeros(img.size(), CV_32FC3);

    // Appliquer le noyeau sur chaque pixel
    for (int y = 0; y < img.rows; y++)
    {
        for (int x = 0; x < img.cols; x++)
        {
            cv::Vec3f pixelValue = cv::Vec3f(0.0f, 0.0f, 0.0f);

            // Parcourir les pixels voisins; dy et dx sont les coordonnées du noyeau
            for (int dy = 0; dy < noyauSize; dy++)
            {
                for (int dx = 0; dx < noyauSize; dx++)
                {
                    // Calcule les coordonnées du voisin
                    int ny = y + dy - padding;
                    int nx = x + dx - padding;

                    // Ignorer les pixels en dehors de l'image
                    if (ny < 0 || ny >= img.rows || nx < 0 || nx >= img.cols)
                    {
                        continue;
                    }

                    // Add weighted contribution from this neighbor
                    cv::Vec3b imageValue = img.at<cv::Vec3b>(ny, nx);
                    for (int c = 0; c < 3; c++)
                    {
                        pixelValue[c] += imageValue[c] * noyau[dy][dx];
                    }
                }
            }

            filtered.at<cv::Vec3f>(y, x) = pixelValue;
        }
    }

    // Convert back to 8-bit image
    cv::Mat result;
    filtered.convertTo(result, CV_8UC3);
    return result;
}

enum class TypeNoyau
{
    Moyenneur,
    Gaussien,
    PasseHaut,
    SobelX,
    SobelY
};

std::vector<std::vector<float>> getNoyau(TypeNoyau type)
{
    switch (type)
    {
    case TypeNoyau::Moyenneur:
        return {
            {1.0f / 9, 1.0f / 9, 1.0f / 9},
            {1.0f / 9, 1.0f / 9, 1.0f / 9},
            {1.0f / 9, 1.0f / 9, 1.0f / 9}};
    case TypeNoyau::Gaussien:
        return {
            {1.0f / 16, 2.0f / 16, 1.0f / 16},
            {2.0f / 16, 4.0f / 16, 2.0f / 16},
            {1.0f / 16, 2.0f / 16, 1.0f / 16}};
    case TypeNoyau::PasseHaut:
        return {
            {-1.0f, -1.0f, -1.0f},
            {-1.0f, 8.0f, -1.0f},
            {-1.0f, -1.0f, -1.0f}};
    case TypeNoyau::SobelX:
        return {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1}};
    case TypeNoyau::SobelY:
        return {
            {-1, -2, -1},
            {0, 0, 0},
            {1, 2, 1}};
    default:
        throw std::invalid_argument("Invalid kernel type");
    }
}

// Calculate the Mean Absolute Error (MAE) between two images
double calculateMAE(const cv::Mat &img1, const cv::Mat &img2)
{
    cv::Mat diff;
    cv::absdiff(img1, img2, diff);
    cv::Scalar mae = cv::mean(diff);
    return (mae[0] + mae[1] + mae[2]) / 3.0; // Average over RGB channels
}

int main()
{
    cv::Mat img = cv::imread("./Donnee2/Images/champ.jpg");
    if (img.empty())
    {
        std::cerr << "Unable to load image.\n";
        return 1;
    }

    // Apply custom convolution with Gaussian kernel
    auto noyauGaussien = getNoyau(TypeNoyau::Gaussien);
    cv::Mat customGaussian = applyConvolution(img, noyauGaussien);

    // Apply OpenCV GaussianBlur
    cv::Mat opencvGaussian;
    cv::GaussianBlur(img, opencvGaussian, cv::Size(3, 3), 0);

    // Apply custom Sobel-X
    auto noyauSobelX = getNoyau(TypeNoyau::SobelX);
    cv::Mat customSobelX = applyConvolution(img, noyauSobelX);

    // Apply OpenCV Sobel
    cv::Mat opencvSobelX;
    cv::Sobel(img, opencvSobelX, CV_8U, 1, 0, 3);

    // Display the results
    // cv::imshow("Original Image", img);
    // cv::imshow("Custom Gaussian", customGaussian);
    // cv::imshow("OpenCV GaussianBlur", opencvGaussian);
    // cv::imshow("Custom Sobel X", customSobelX);
    // cv::imshow("OpenCV Sobel X", opencvSobelX);

    // Calculate numerical differences
    double maeGaussian = calculateMAE(customGaussian, opencvGaussian);
    double maeSobelX = calculateMAE(customSobelX, opencvSobelX);

    std::cout << "MAE (Gaussian): " << maeGaussian << "\n";

    std::cout << "MAE (Sobel-X): " << maeSobelX << "\n";

    return 0;
}