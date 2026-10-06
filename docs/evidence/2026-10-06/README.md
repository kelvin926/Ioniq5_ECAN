# 2026-10-06 관측 기록

Windows 노트북에서 얻은 날짜별 원본 JSON 데이터를 보존하고 저장소 규칙에 맞춰 LF 개행을
사용합니다. 원본 파일과 전달 파일의 SHA-256은 state.json에 각각 기록했습니다. 아래 순서는
같은 날짜 안의 작업 순서이며, 서로 다른 Panda uptime/재연결 구간이 포함됩니다.
최신 상태는 마지막 **퓨즈 교체 후 passive 관측**입니다. 과거 값을 현재 상태로 취급하지
않습니다. 측정 방법과 해석은 각 JSON 및 [차량 인수인계](../../vehicle_handoff.md)에 있습니다.

| 순서 | 원본 | 확인 범위 |
| --- | --- | --- |
| 1 | [MDPS passive](vehicle-observation-20261006-mdps.json) | 퓨즈 교체 전 MDPS/PA/ACI broadcast; ignition=0, LFA/SCC 미수신 |
| 2 | [조향 준비 상태](steering-readiness-20261006.json) | 퓨즈 교체 전 정차/brake와 ignition=0; 실제 조향 미실행 |
| 3 | [DTC 조회](vehicle-dtcs-20261006.json) | HVAC raw 923413/status 09, 다른 후보 timeout |
| 4 | [ADAS 집중 조회](adas-diagnostics-20261006.json) | ECU 식별/DTC 조회, ADAS 후보 timeout, HVAC 참조 응답 |
| 5 | [한 번의 DTC 삭제](dtc-clear-20261006.json) | 사용자 요청; HVAC 수락 후 동일 코드 재확인, ADAS 삭제 미확인 |
| 6 | [퓨즈 교체 후 passive](panda-post-fuse-20261006.json) | NORMAL/ignition=1, LFA 100 Hz/SCC 50 Hz, 선택 CRC 오류/TX 0 |

원래 `build/` 아래에 있었던 기록입니다. JSON 내부의 Windows 경로와 원래 scratch script
이름은 provenance이며 차량 컴퓨터의 실행 경로가 아닙니다. `build/` 임시 helper와 빌드
산출물은 Git 전달에서 제외했습니다. 고장코드 삭제나 제어를 자동으로 다시 실행하는
절차가 아니며 퓨즈 교체 후 DTC/UDS 재조회 결과는 없습니다.
