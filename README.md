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
영상처리의 알고리즘인 Harris Corner Detection을 3차원에서 활용할 수 있도록 하는 알고리즘

<img src = "https://user-images.githubusercontent.com/30585313/174285771-6befc042-6617-402d-965e-739d024939b7.png" width="50%" height="50%">
결과물

### 2-2. Non-Maximum Suppression
<img src = "https://user-images.githubusercontent.com/30585313/174285771-6befc042-6617-402d-965e-739d024939b7.png" width="50%" height="50%">
딥러닝 영상처리에서 중복도를 줄이기 위해 사용되는 알고리즘

<img src = "https://user-images.githubusercontent.com/30585313/174285771-6befc042-6617-402d-965e-739d024939b7.png" width="50%" height="50%">
<img src = "https://user-images.githubusercontent.com/30585313/174285771-6befc042-6617-402d-965e-739d024939b7.png" width="50%" height="50%">

Intersection of Union은 두 keypoint의 거리 및 방향의 유사성으로 대체

<img src = "https://user-images.githubusercontent.com/30585313/174285771-6befc042-6617-402d-965e-739d024939b7.png" width="50%" height="50%">
<img src = "https://user-images.githubusercontent.com/30585313/174285771-6befc042-6617-402d-965e-739d024939b7.png" width="50%" height="50%">
결과물

### 2-3. Vertex Type Detection
<img src = "https://user-images.githubusercontent.com/30585313/174285771-6befc042-6617-402d-965e-739d024939b7.png" width="50%" height="50%">
Keypoint의 특성을 판단하기 위한 알고리즘

<img src = "https://user-images.githubusercontent.com/30585313/174285771-6befc042-6617-402d-965e-739d024939b7.png" width="50%" height="50%">
<img src = "https://user-images.githubusercontent.com/30585313/174285771-6befc042-6617-402d-965e-739d024939b7.png" width="50%" height="50%">
<img src = "https://user-images.githubusercontent.com/30585313/174285771-6befc042-6617-402d-965e-739d024939b7.png" width="50%" height="50%">
결과물


## 3. 기능 구현
### 3-1. Procedural Animation
<img src = "https://user-images.githubusercontent.com/30585313/174285771-6befc042-6617-402d-965e-739d024939b7.png" width="50%" height="50%">
Inverse Kinematic을 이용하여 암벽등반하는 케릭터 애니메이션을 구현

### 3-2. Procedural Spline Generation
<img src = "https://user-images.githubusercontent.com/30585313/174285771-6befc042-6617-402d-965e-739d024939b7.png" width="50%" height="50%">
<img src = "https://user-images.githubusercontent.com/30585313/174285771-6befc042-6617-402d-965e-739d024939b7.png" width="50%" height="50%">
Harris 3D 연산을 통한 Point를 이용하여 생성한 Line

## 4. 최종 결과 
### 4-1. Procedural Animation
<img src = "https://user-images.githubusercontent.com/30585313/183239606-e3e10825-24ba-4611-82d4-f6cd6d8480d7.gif" width="50%" height="50%">
<img src = "https://user-images.githubusercontent.com/30585313/183240964-277e3d01-ee7f-4e77-b5b9-8bdc822dcaed.gif" width="50%" height="50%">
<img src = "https://user-images.githubusercontent.com/30585313/183240975-3416c43a-7f37-4145-b156-1141d6ff8106.gif" width="50%" height="50%">

### 4-2. Procedural Spline Generation
<img src = "https://user-images.githubusercontent.com/30585313/183239580-5e06a2cf-ceba-427e-9df8-1e74593f795e.gif" width="50%" height="50%">
<img src = "https://user-images.githubusercontent.com/30585313/183239595-7f28b9ee-be49-4751-9ca7-d21fbe3be5ba.gif" width="50%" height="50%">
<img src = "https://user-images.githubusercontent.com/30585313/183239597-970b62da-b91a-4f6d-ad65-d882bd5664f5.gif" width="50%" height="50%">
<img src = "https://user-images.githubusercontent.com/30585313/183239600-283e76e4-c234-4db9-b8ea-51a8da8bfa6e.gif" width="50%" height="50%">
<img src = "https://user-images.githubusercontent.com/30585313/183239602-61db2ba0-b174-4da7-b9fd-25f5c23f044f.gif" width="50%" height="50%">


## 5. 결론
애니메이션: 순간 어색함
Spline: 뚫려 생성된 모습

상황에 따라 적절한 Harris 3D 활용이 필요하다.
