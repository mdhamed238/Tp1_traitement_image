#include <iostream>
#include <opencv2/opencv.hpp>

std::vector<uchar> getVoisins(const cv::Mat &img, int x, int y)
{
    std::vector<uchar> voisins;
    for (int i = x - 1; i <= x + 1; i++)
    {
        for (int j = y - 1; j <= y + 1; j++)
        {
            if (i >= 0 && i < img.rows && j >= 0 && j < img.cols)
            {
                voisins.push_back(img.at<uchar>(i, j));
            }
        }
    }
    return voisins;
}

// Calculer le mediane des voisins d'un pixel donné
uchar mediane(const cv::Mat &img, int x, int y)
{
    std::vector<uchar> voisins = getVoisins(img, x, y);
    std::sort(voisins.begin(), voisins.end());
    return voisins[voisins.size() / 2];
}

int main()
{
    cv::Mat img = cv::imread("./Donnee2/Images/champ.jpg", cv::IMREAD_GRAYSCALE);
    // Copier l'image avant le filtrage
    cv::Mat img2 = img.clone();
    // Afficher l'image avant le filtrage
    // cv::imshow("Image1", img);
    std::vector<uchar> voisins = getVoisins(img, 1, 1);

    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            img.at<uchar>(i, j) = mediane(img, i, j);
        }
    }

    // Afficher l'image
    // cv::imshow("Image2", img);
    // cv::waitKey(0); // Wait for a key press to close the window

    return 0;
}

/*
    x x x
    x p x
    x x x
*/