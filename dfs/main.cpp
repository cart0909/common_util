#include <deque>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
using namespace std;

void dfs(const cv::Mat& img,
    cv::Mat& traversal_map,
    const cv::Point& pt,
    const std::vector<cv::Point2i>& policy,
    std::vector<cv::Point2i>& contour)
{
    traversal_map.at<uchar>(pt.y, pt.x) = 1;
    contour.push_back(pt);
    for (int i = 0, n = policy.size(); i < n; ++i) {
        cv::Point pt1 = pt + policy[i];
        if (pt1.x < 0 || pt1.y < 0 || pt1.x >= img.cols || pt1.y >= img.rows) {
            continue;
        }
        if (img.at<uchar>(pt1.y, pt1.x) != 0 && traversal_map.at<uchar>(pt1.y, pt1.x) == 0)
            dfs(img, traversal_map, pt1, policy, contour);
    }
}

void dfs_iterative(const cv::Mat& img, cv::Mat& traversal_map,
    const cv::Point& root_pt, const std::vector<cv::Point2i>& policy,
    std::vector<cv::Point2i>& contour)
{
    std::deque<cv::Point2i> record;
    record.push_back(root_pt);

    while (!record.empty()) {
        cv::Point pt = record.back();
        record.pop_back();

        if (img.at<uchar>(pt.y, pt.x) != 0 && traversal_map.at<uchar>(pt.y, pt.x) == 0) {
            traversal_map.at<uchar>(pt.y, pt.x) = 1;
            contour.push_back(pt);
        } else {
            continue;
        }

        for (int n = policy.size(), i = n; i >= 0; --i) {
            cv::Point pt1 = pt + policy[i];
            if (pt1.x < 0 || pt1.y < 0 || pt1.x >= img.cols || pt1.y >= img.rows) {
                continue;
            }

            record.push_back(pt1);
        }
    }
}

int main()
{
    cv::Mat img = cv::imread("/home/liu/workspace/dfs/1.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat edge;
    cv::Canny(img, edge, 100, 200);
    cv::Mat traversal_map = cv::Mat::zeros(img.size(), CV_8U);

    std::vector<cv::Point2i> policy{ { -1, -1 }, { 0, -1 }, { 1, -1 },
        { -1, 0 }, { 1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 } };

    std::vector<std::vector<cv::Point2i>> contours;

    for (int col_index = 0, cols = img.cols; col_index < cols; ++col_index) {
        for (int row_index = 0, rows = img.rows; row_index < rows; ++row_index) {
            if (edge.at<uchar>(row_index, col_index) != 0 && traversal_map.at<uchar>(row_index, col_index) == 0) {
                contours.emplace_back();
                dfs_iterative(edge, traversal_map, cv::Point2i(col_index, row_index), policy, contours.back());
            }
        }
    }

    cv::Mat display(edge.size(), CV_8U, cv::Scalar(255));
    cv::imshow("display", edge);
    cv::waitKey(0);
    for (int i = 0, n = contours.size(); i < n; ++i) {
        for (int j = 0, m = contours[i].size(); j < m; ++j) {
            display.at<uchar>(contours[i][j].y, contours[i][j].x) = 0;
            cv::imshow("display", display);
            cv::waitKey(1);
        }
    }
    cv::waitKey(0);
    return 0;
}
