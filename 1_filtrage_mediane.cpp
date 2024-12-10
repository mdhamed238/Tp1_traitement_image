#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <vector>

// renvoie les voisins d'un pixel donné
std::vector<cv::Vec3b> getVoisins(const cv::Mat &img, int x, int y)
{
    std::vector<cv::Vec3b> voisins;
    for (int i = x - 1; i <= x + 1; i++)
    {
        for (int j = y - 1; j <= y + 1; j++)
        {
            if (i >= 0 && i < img.rows && j >= 0 && j < img.cols)
            {
                voisins.push_back(img.at<cv::Vec3b>(i, j));
            }
        }
    }
    return voisins;
}

// Calculer le mediane des voisins d'un pixel donné
cv::Vec3b mediane(const cv::Mat &img, int x, int y)
{
    std::vector<cv::Vec3b> voisins = getVoisins(img, x, y);
    std::sort(voisins.begin(), voisins.end(), [](const cv::Vec3b &a, const cv::Vec3b &b)
              { return cv::sum(a)[0] < cv::sum(b)[0]; });
    return voisins[voisins.size() / 2];
}

int main()
{
    cv::Mat img = cv::imread("./Donnee2/Images/champ.jpg");
    if (img.empty())
    {
        std::cerr << "Could not read the image: ./Donnee2/Images/champ.jpg" << std::endl;
        return 1;
    }

    // Afficher l'image avant le filtrage
    cv::imshow("Image1", img);

    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            img.at<cv::Vec3b>(i, j) = mediane(img, i, j);
        }
    }

    // Afficher l'image
    cv::imshow("Image2", img);
    cv::waitKey(0); // Wait for a key press to close the window

    return 0;
}

/*
    x x x
    x p x
    x x x
*/