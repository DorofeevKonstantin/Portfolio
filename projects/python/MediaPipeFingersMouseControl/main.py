import cv2
import numpy as np
import mediapipe as mp
import pyautogui
import math

def distance(Sx, Sy, Ex, Ey):
    return math.sqrt((abs(Sx - Ex)) ** 2 + (abs(Sy - Ey)) ** 2)

screenWidth, screenHeight = pyautogui.size()
previewWidth = int(screenWidth / 3)
previewHeight = int(screenHeight / 3)
winName = "preview"
cv2.namedWindow(winName)
cv2.moveWindow(winName, 0, 0)

mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles
mp_hands = mp.solutions.hands

cap = cv2.VideoCapture(0)
cap.set(3, 640)
cap.set(4, 360)

leftClickTimes = 0
dragged = False

with mp_hands.Hands(
        model_complexity=0,
        min_detection_confidence=0.8,
        min_tracking_confidence=0.8) as hands:
    while cap.isOpened():
        success, frame = cap.read()
        if not success:
            print("Ignoring empty camera frame.")
            continue
        resized = cv2.resize(frame, (screenWidth, screenHeight))
        resized = cv2.flip(resized, 1)
        resized.flags.writeable = False
        resized = cv2.cvtColor(resized, cv2.COLOR_BGR2RGB)
        results = hands.process(resized)
        resized.flags.writeable = True
        resized = cv2.cvtColor(resized, cv2.COLOR_RGB2BGR)
        preview = cv2.resize(resized, (previewWidth, previewHeight))
        if results.multi_hand_landmarks:
            for hand_landmarks in results.multi_hand_landmarks:
                mp_drawing.draw_landmarks(
                    preview,
                    hand_landmarks,
                    mp_hands.HAND_CONNECTIONS,
                    mp_drawing_styles.get_default_hand_landmarks_style(),
                    mp_drawing_styles.get_default_hand_connections_style())
            hand_landmarks = results.multi_hand_landmarks[0]
            tipX = hand_landmarks.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP].x * screenWidth
            tipY = hand_landmarks.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP].y * screenHeight

            bigFingerX = hand_landmarks.landmark[mp_hands.HandLandmark.THUMB_TIP].x * screenWidth
            bigFingerY = hand_landmarks.landmark[mp_hands.HandLandmark.THUMB_TIP].y * screenHeight

            middleFingerX = hand_landmarks.landmark[mp_hands.HandLandmark.MIDDLE_FINGER_TIP].x * screenWidth
            middleFingerY = hand_landmarks.landmark[mp_hands.HandLandmark.MIDDLE_FINGER_TIP].y * screenHeight

            if distance(bigFingerX, bigFingerY, tipX, tipY) < 80:
                dragged = True
            else:
                dragged = False
                if distance(middleFingerX, middleFingerY, tipX, tipY) < 90:
                    pyautogui.rightClick(tipX, tipY)

            if not dragged:
                cv2.putText(preview, "Move", (20, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255))
                pyautogui.mouseUp()
                pyautogui.moveTo(tipX, tipY)
            else:
                cv2.putText(preview, "Drag", (20, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255))
                pyautogui.mouseDown(tipX, tipY)
                pyautogui.moveTo(tipX, tipY)
        else:
            cv2.putText(preview, "No hand in webcam", (20, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255))

        cv2.imshow(winName, preview)
        if cv2.waitKey(1) & 0xFF == 27:
            break

cap.release()