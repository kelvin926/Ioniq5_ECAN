# 2026-10-06 관측 기록

Windows 노트북의 기존 원본 JSON과 이후 Ubuntu 차량 컴퓨터의 수신 검사 결과를 보존합니다.
저장소 규칙에 맞춰 LF 개행을 사용합니다. 2026-10-07 사용자가 선택한 개인정보와 장치
식별정보를 공개 사본에서 제거했습니다. CAN/UDS payload와 측정값은 유지했습니다.
state.json의 현재 사본 SHA-256은 갱신했으며 기존 원본 hash는 별도 provenance입니다. 아래 순서는
같은 날짜 안의 작업 순서이며, 서로 다른 Panda uptime/재연결 구간이 포함됩니다.
차량 수신 검사 이후 Cabana는 사용자 요청으로 꺼진 상태를 확인했습니다. 최신 관측은
사용자 승인으로 수행한 **수정 후 재연결/DTC 조회와 한 번의 삭제 후 재발 검사**입니다. 과거 값을 현재 상태로 취급하지
않습니다. 측정 방법과 해석은 각 JSON 및 [차량 인수인계](../../vehicle_handoff.md)에 있습니다.

| 순서 | 원본 | 확인 범위 |
| --- | --- | --- |
| 1 | [MDPS passive](vehicle-observation-20261006-mdps.json) | 퓨즈 교체 전 MDPS/PA/ACI broadcast; ignition=0, LFA/SCC 미수신 |
| 2 | [조향 준비 상태](steering-readiness-20261006.json) | 퓨즈 교체 전 정차/brake와 ignition=0; 실제 조향 미실행 |
| 3 | [DTC 조회](vehicle-dtcs-20261006.json) | HVAC raw 923413/status 09, 다른 후보 timeout |
| 4 | [ADAS 집중 조회](adas-diagnostics-20261006.json) | ECU 식별/DTC 조회, ADAS 후보 timeout, HVAC 참조 응답 |
| 5 | [한 번의 DTC 삭제](dtc-clear-20261006.json) | 사용자 요청; HVAC 수락 후 동일 코드 재확인, ADAS 삭제 미확인 |
| 6 | [퓨즈 교체 후 passive](panda-post-fuse-20261006.json) | NORMAL/ignition=1, LFA 100 Hz/SCC 50 Hz, 선택 CRC 오류/TX 0 |
| 7 | [차량 컴퓨터 CAN 수신 검사](vehicle-computer-can-reception-20261006.json) | 읽기 전용 preflight는 누적 overflow로 FAIL; Cabana live 로그의 ECAN 8,620개/약 3초, LFA 100 Hz/SCC 50 Hz, 선택 CRC 오류 0, 관측 중 TX/overflow 증가 0, 이후 종료 및 앱 바로가기 생성 |
| 8 | [I5R1 USB-only flash/bench](command-session-usb-20261006.json) | 하네스 분리 확인, 앱 flash/서명/capability 검증, preflight PASS, standby 송신 3개 차단/TX 증가 0. 주행 중 재인계 실차 시험은 미실시 |
| 9 | [정차 ROS/ECU 시험](stationary-ros-test-20261006.json) | 입력 수신/idle 단절 복귀, UDS 응답과 LFA/SCC quiet/통신 복구. 인계 후 Panda CAN 준비 실패로 ACTIVE 미진입, SCC 통신 이상 신호 잔존. 구현/firmware 변경 없음 |
| 10 | [복구 수정/플래시](ecan-recovery-fix-20261006.json) | 첫 RX와 안전 tick 경쟁 재현/수정, host 초기 동기화/버튼 대기/2단계 복구. firmware 14개/host 18개 관련 테스트와 빌드, 새 앱 USB-only flash/서명 검증, preflight PASS/물리 TX 0. 수정 후 실차 ACTIVE와 ACC 해소 미확인 |
| 11 | [수정 후 DTC 재발](post-fix-ecu-dtcs-20261006.json) | 재연결 ROS 수신/ready 정상, 무출력. 0x730/7D0 DTC 조회 및 승인된 0x730 1회 삭제는 54 수락. 2초 뒤 세 status89 기록 재발/ACCEnable3 유지, 추가 삭제/진단/제어 중단 |

1~6은 원래 `build/` 아래에 있었던 기록입니다. 개인 Windows 경로와 실제 장치 일련번호는
공개 사본에서 제거했습니다. 남은 scratch script 이름은 provenance이며 실행 경로가 아닙니다.
`build/` 임시 helper와 빌드
산출물은 Git 전달에서 제외했습니다. 고장코드 삭제나 제어를 자동으로 다시 실행하는
절차가 아닙니다. 9번 정차 시험은 UDS 통신 응답만 기록하며, 이후 DTC 재조회와 승인된
한 번의 삭제/재발 결과는 11번에 별도로 보존했습니다.
