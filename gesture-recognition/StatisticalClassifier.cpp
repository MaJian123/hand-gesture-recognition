#include "StatisticalClassifier.h"

StatisticalClassifier::StatisticalClassifier(TrainableStatModel *statisticalModel)
        : statisticalModel(statisticalModel)
{
}

TrainableStatModel *StatisticalClassifier::getStatisticalModel() const{
    return this->statisticalModel;
}

float StatisticalClassifier::predict(const Mat &segmentedHand) {
    Mat caracteristicVector = this->caracteristicVector(segmentedHand);
    cout<<"Caracteristic vector computed"<<endl;
        
    return this->statisticalModel->predict(caracteristicVector);
}

int StatisticalClassifier::numberOfFingersFromClassifierResult(float classifierResult) {
    return round(classifierResult);
}

int StatisticalClassifier::numberOfFingers(Mat &segmentedHand) {
    Mat handCaracteristicVector = this->caracteristicVector(segmentedHand);
    float classifierResult = 
          this->predict(handCaracteristicVector);
    return this->numberOfFingersFromClassifierResult(classifierResult);
}

void StatisticalClassifier::train(
        const vector<Mat> &segmentedHands,
        const vector<int> &expectedClass) {
    assert(segmentedHands.size() == expectedClass.size());
    Mat trainData(segmentedHands.size(), this->caracteristicVectorLength(), CV_32F);
    Mat expectedResponses(1, segmentedHands.size(), CV_32F);

    for (int i = 0; i < segmentedHands.size(); i++) {
        Mat currentCaracteristicVector = this->caracteristicVector(segmentedHands[i]);
        currentCaracteristicVector.copyTo(trainData.row(i));
        expectedResponses.at<float>(0, i) = expectedClass[i];
    }
    
    this->statisticalModel->train(trainData, expectedResponses);
}

void StatisticalClassifier::load(const char *filepath) {
    this->statisticalModel->getStatModel()->load(filepath);
}

void StatisticalClassifier::save(const char *filepath) {
    this->statisticalModel->getStatModel()->save(filepath);
}