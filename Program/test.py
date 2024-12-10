import cv2
import numpy as np
from tensorflow.keras.models import load_model
from tensorflow.keras.preprocessing.image import img_to_array
import os

# 전역 모델, 캐스케이드 로딩(초기화는 한 번만)
model_path = './cnn_model_04.h5'
model = load_model(model_path)
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

# 이미지 파일 경로 설정
image_path = './LastImg.jpg'

def predict():
    """고정된 이미지 경로에 대해 얼굴 인식 및 모델 예측을 수행하고, 신뢰도 반환."""
    if os.path.exists(image_path):
        image = cv2.imread(image_path)
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        faces = face_cascade.detectMultiScale(
            gray, scaleFactor=1.1, minNeighbors=5, minSize=(64, 64)
        )

        if len(faces) > 0:
            x, y, w, h = faces[0]  # 첫 번째 얼굴만 선택
            face = image[y:y+h, x:x+w]
            face = cv2.resize(face, (64, 64))  # 모델 입력 크기
            face = img_to_array(face) / 255.0
            face = np.expand_dims(face, axis=0)  # 배치 차원 추가

            # 예측
            prediction = model.predict(face)
            confidence = prediction[0][0] * 100
            return confidence
        else:
            return -1.0
    else:
        return -1.0

def zako():
    return "zako"