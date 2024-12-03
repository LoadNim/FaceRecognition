import cv2
import numpy as np
from tensorflow.keras.models import load_model
from tensorflow.keras.preprocessing.image import img_to_array
import os

# 모델 로드
model_path = './cnn_model_04.h5'
model = load_model(model_path)

# 얼굴 검출기 초기화
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

# 이미지 파일 경로 설정
image_path = './LastImg.jpg'  # 비교할 이미지의 경로로 변경

# 이미지 불러오기
if os.path.exists(image_path):
    image = cv2.imread(image_path)
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(64, 64))
    # 첫 번째 얼굴만 예측 수행
    if len(faces) > 0:
        x, y, w, h = faces[0]  # 첫 번째 얼굴만 선택
        face = image[y:y+h, x:x+w]
        face = cv2.resize(face, (64, 64))  # 모델의 입력 형상에 맞게 (64, 64)로 조정
        face = img_to_array(face) / 255.0
        face = np.expand_dims(face, axis=0)  # 배치를 위해 차원 추가 (1, 64, 64, 3)

        # 예측
        
        prediction = model.predict(face)
        confidence = prediction[0][0] * 100  # 신뢰도 % 계산
        with open('log.txt', 'w') as file:
            file.write(f'{confidence:.2f}')
    else:
        with open('log.txt', 'w') as file:
            file.write('-1')