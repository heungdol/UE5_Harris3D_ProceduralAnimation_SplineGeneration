# Harris 3D과 Procedural Animation을 활용한 등산모션 제작
<img src = "https://user-images.githubusercontent.com/30585313/174286112-9d2da32b-fa5a-43d5-9a42-48f127dd3e89.gif" width="50%" height="50%">
<br>
2022년 1학기 소프트웨어융합캡스톤디자인 프로젝트

## 1. 개요
### 1-1. 배경
<img src = "https://user-images.githubusercontent.com/30585313/174286521-bcb7c424-cd11-41bf-af16-67b6b92aa503.gif" width="50%" height="50%">
<br>
최근 나오는 게임들을 보면 캐릭터들의 움직임이 되게 입체적인 것을 확인할 수 있다. XY 평면으로만 이동하지 않고 높이 Z축으로도 이동한다는 점이다. 그렇기에 자연스럽게 벽과 상호작용하는 액션(벽타기, 오르기... 등)의 애니메이션이 필요하게 된다. 이 때 벽화 상호작용하는 액션 중 암벽등반이라는 구체적인 상황을 설정함으로써, 이의 자연스러운 애니메이션을 연출하기 위해 프로젝트를 진행하게 되었다.

### 1-2. 목표
보다 자연스러운 암벽등반 애니메이션을 연출할 수 있도록 한다.

### 1-3. 기대
Harris 3D를 비롯한 3D메쉬의 keypoint를 검출하는 알고리즘이 Point Matching 분야 뿐만 아니라 Procedural Animation에서도 활용할 수 있음을 보이고자 한다.

## 2. 활용 알고리즘 및 기타

### 2-1. Harris 3D
영상처리의 알고리즘인 Harris Corner Detection을 3차원에서 활용할 수 있도록 하는 알고리즘이다.

### 2-2. Procedural Animation
Inverse Kinematic을 이용하여 케릭터 애니메이션을 구현하는 기법이다.

### 2-3. Megascans
Quixel Bridge에서 제공하는 3D 스캔 데이터이다.

#### 2-3-1. 자연물
- SHARP CLIFF <BR>
<img src = "https://user-images.githubusercontent.com/30585313/174285771-6befc042-6617-402d-965e-739d024939b7.png" width="50%" height="50%">

- FOREST ROCK WALL <BR>
<img src = "https://user-images.githubusercontent.com/30585313/174285772-6097c839-e9a8-4dcb-8911-dc941c6aca45.png" width="50%" height="50%">

- ROCKY CLIFF <BR>
<img src = "https://user-images.githubusercontent.com/30585313/174285775-aa4a6d98-f241-4576-aa89-55e745c13e2f.png" width="50%" height="50%">

#### 2-3-2. 인공물
- ROMAN STATUE <BR>
<img src = "https://user-images.githubusercontent.com/30585313/174285760-1328aacd-2a23-4774-b122-527983769cb7.png" width="50%" height="50%">

- ROMAN GRAVE STONE <BR>
<img src = "https://user-images.githubusercontent.com/30585313/174285763-6675495c-cd54-4c9e-b113-9b3133001c3d.png" width="50%" height="50%">

- ROMAN MARBLE ORNATE PLINTH <BR>
<img src = "https://user-images.githubusercontent.com/30585313/174285765-82f90d4a-0d44-47df-85e1-a2622214cf60.png" width="50%" height="50%">

- ROMAN RED BRICK COLUMN <BR>
<img src = "https://user-images.githubusercontent.com/30585313/174285769-7779cee5-f459-4482-9111-d3d4adcd1758.png" width="50%" height="50%">


## 3. 1차 구현 및 결과 (소융캡디 최종 발표)

### 3-1. Harris 3D Keypoint 검출
#### 3-1-1. 자연물
- SHARP CLIFF (Vertex Ring Number = 3 / Harris Number = 0.2) <BR>
<img src = "https://user-images.githubusercontent.com/30585313/174285707-10dadd65-6ba1-421e-900c-59a68b56538a.gif" width="50%" height="50%">

    
- FOREST ROCK WALL (Vertex Ring Number = 3 / Harris Number = 0.1) <BR>
<img src = "https://user-images.githubusercontent.com/30585313/174285703-e760e6de-d994-48cf-9dc1-197b7f86b542.gif" width="50%" height="50%">

    
- ROCKY CLIFF (Vertex Ring Number = 3 / Harris Number = 0.2) <BR>
<img src = "https://user-images.githubusercontent.com/30585313/174285695-3a5090fb-6707-4502-a4f6-a276f8b37e3f.gif" width="50%" height="50%">

    
#### 3-1-2. 인공물
- ROMAN STATUE (Vertex Ring Number = 3 / Harris Number = 0.05) <BR>
<img src = "https://user-images.githubusercontent.com/30585313/174285710-7c1c95c7-4887-4328-9ea4-99f54c13fe36.gif" width="50%" height="50%">
 
    
- ROMAN GRAVE STONE (Vertex Ring Number = 3 / Harris Number = 0.245) <BR>
<img src = "https://user-images.githubusercontent.com/30585313/174285713-e44eea65-6692-4d9d-a1da-b342728d0218.gif" width="50%" height="50%">

    
- ROMAN MARBLE ORNATE PLINTH (Vertex Ring Number = 3 / Harris Number = 0.2) <BR>
<img src = "https://user-images.githubusercontent.com/30585313/174285716-64662df2-17d4-4cfb-8833-6ed764e543e6.gif" width="50%" height="50%">

    
- ROMAN RED BRICK COLUMN (Vertex Ring Number = 3 / Harris Number = 0.2) <BR>
<img src = "https://user-images.githubusercontent.com/30585313/174285719-57a778e5-4a13-44ca-838b-27dd1846ac6a.gif" width="50%" height="50%">

    
### 3-2. Procedural Animation
#### 3-2-1. Third Person Character <br>
<img src = "https://user-images.githubusercontent.com/30585313/174285821-16b3f77e-a561-4810-bb23-620719eb1614.png" width="50%" height="50%">
    - 메인 캐릭터 Actor
    - 4개의 IK Effector와 Pelvis Actor Component를 가짐
    - 위의 5개 Actor Component를 관할하는 형식
    - 각각의 이동 조건판단 및 Keypoint Location / Normal을 받아 실질적인 이동 계산
    
#### 3-2-2. IK Effector <br>
<img src = "https://user-images.githubusercontent.com/30585313/174285832-6dfb88d7-e064-4889-beb0-dd14b2cdc3d2.png" width="50%" height="50%">
    - Hands와 Foots를 관리하는 Actor Component
    - Keypoint의 Normal를 내적시켜 잡거나 딛을 수 있는 Keypoint인지 판단
    
#### 3-2-3. Pelvis Transform <br>
<img src = "https://user-images.githubusercontent.com/30585313/174285827-71be86e9-819d-4146-bcd7-533e5438efe1.png" width="50%" height="50%">
    - Pelvis의 Transform을 관리하는 Actor Component
    - 이동할 수 있는 구역 판단
 
### 3-3. 암벽등반 애니메이션 구현
#### 3-3-1. 자연물
<img src = "https://user-images.githubusercontent.com/30585313/174286112-9d2da32b-fa5a-43d5-9a42-48f127dd3e89.gif" width="50%" height="50%">
<img src = "https://user-images.githubusercontent.com/30585313/174286120-15dfddbe-765d-4bda-bb48-60e589e2d22d.gif" width="50%" height="50%">
<img src = "https://user-images.githubusercontent.com/30585313/174286123-6c261a81-ae0e-4176-b9db-af4d6c4716da.gif" width="50%" height="50%">
    
#### 3-3-2. 인공물
<img src = "https://user-images.githubusercontent.com/30585313/174285430-44f5e0ca-84dc-4114-a964-ddc6c02ff3fa.gif" width="50%" height="50%">
<img src = "https://user-images.githubusercontent.com/30585313/174285404-397abcc6-2b07-4a48-b192-5406f889d508.gif" width="50%" height="50%">
<img src = "https://user-images.githubusercontent.com/30585313/174285417-0db23a3e-5216-4bed-b638-867f31dbe833.gif" width="50%" height="50%">
<img src = "https://user-images.githubusercontent.com/30585313/174285421-8951beb0-2e42-4a9d-86e8-7d7609edc464.gif" width="50%" height="50%">
<img src = "https://user-images.githubusercontent.com/30585313/174285426-909bffe4-ad10-4e27-959b-f4cf079182c4.gif" width="50%" height="50%">

<br>
특징과 특징이 아닌 구역이 잘 구분되는 인공물 메쉬가 비교적 Keypoint 검출하는 데에 자연스러워 보인다. <br>
그러한 이유로 타당한 Vertex를 잡거나 딛는 움직임을 보인다. <br>
    
## 4. 피드백 및 추가 구현 사항 (소융캡디 최종 발표)
    - 피드백: 검출시 Keypoint들이 가까이 겹쳐져 있는 부분 수정요함
        - 활용할 수 있는 알고리즘으로썬 NMS
    - 피드백: 자연물을 보면 Keypoint들이 제대로 검출되지 않았다고 했는데, 두 개의 임계치를 수정해서 테스트해볼 것
        - Window Size -> Vertex Ring Number
        - Harris Operator -> Fraction of the Diagonal
    - 피드백: 발표를 할때 IK에 대해 좀 더 이야기 했으면 좋았을 것
    
    - 기타: 캐릭터 컨트롤러 조작감 수정 및  Locomotion 추가
    - 기타: 손과 발의 각도 수정
    - 기타: Github 페이지 수정하기
    - 기타: 발표 때 Keypoint 이야기할 때 Wireframe mode로 설명했으면 더 좋았을 것
    - 기타: 발표 때 파이프라인을 들어 설명했으면 좋았을 것
    - 기타: 정상에 다다를시 올라갈 수 있도록 캐릭터 컨트롤러 수정
    - 기타: 인터페이스가 잘 되어 있는 UE5로 업그레이드 하기
        - UE5에서도 C++ 빌드할 수 있도록 환경 구성할 것
    - 기타: Harris 3D 뿐만 아니라 다른 Keypoint 검출하는 알고리즘을 조사해볼 것
    
## 5. 피드백 수용 및 개선
추후 진행할 예정

## 6. 2차 구현 및 결과
추후 진행할 예정
