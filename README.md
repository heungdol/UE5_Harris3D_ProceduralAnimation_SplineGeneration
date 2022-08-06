# Harris 3D를 활용한 Procedural Climbing Animation 및 Procedural Spline Generation 구현
2022년 1학기 경희대X펄어비스 인재양성 프로그램 결과물

[![Video Label](http://img.youtube.com/vi/vLYE0bL-zPY/0.jpg)](https://youtu.be/vLYE0bL-zPY)
<br>
유튜브 영상

## 1. 개요
### 1-1. 목표
- 3D 엔진에서 활용할 수 있는 게임 기술을 개발할 수 있도록 한다.
- 본 프로젝트에서는 3D Keypoint 검출 알고리즘을 활용하여 사실적인 Procedural Animation 및 Spline Generation을 구현할 수 있도록 한다.

### 1-2. 사용 엔진
Unreal Engine 5
- Control Rig Plugin 정식지원
- 퀄리티 높은 렌더링 결과

## 2. 알고리즘
### 2-1. Harris 3D
이미지 -> 돌 , 의자
영상처리의 알고리즘인 Harris Corner Detection을 3차원에서 활용할 수 있도록 하는 알고리즘

### 2-2. Non-Maximum Suppression
이미지 -> NMS 설명
-> 의자 결과
딥러닝 영상처리에서 중복도를 줄이기 위해 사용되는 알고리즘

### 2-3. Vertex Type Detection
이미지 -> Vertex 벡터 연산 설명
이미지 -> 돌다리, 
Keypoint의 특성을 판단하기 위한 알고리즘

## 3. 기능 구현
### 3-1. Procedural Animation
이미지 -> 일반 등반 모습
Inverse Kinematic을 이용하여 케릭터 애니메이션을 구현하는 기법

### 3-2. Procedural Spline Generation
이미지 -> 움직여서 Spline 생성하는 모습
Point와 Point를 이어 Line을 만드는 Unreal Engine의 기본 Actor이다.

## 4. 최종 결과 
### 4-1. Procedural Animation
이미지 추가 예정
- 마네킹 등반

### 4-2. Procedural Spline Generation
이미지
- 스즈메
- 고드름
- 등나무
- 크리스마스

## 5. 결론
애니메이션: 순간 어색함
Spline: 뚫려 생성된 모습

상황에 따라 적절한 Harris 3D 활용이 필요하다.
