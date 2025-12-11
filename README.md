# esp32s3-lvgl-terminal

基于 **LVGL** 和 **FreeRTOS** 的 **ESP32-S3** 多功能智能终端，集成：

- Wi-Fi 扫描与连接管理
- 时间同步与多日天气展示
- 串口终端
- SD卡文件与本地MP3音乐播放
- 语音识别 + 语音合成（百度实时 ASR + TTS）
- 文本对话（MiniMax / 可扩展其他大模型接口）
- LVGL 小游戏（2048、植物大战僵尸、消消乐、羊了个羊）
---

## 1. 功能概述
- **GUI 框架**：采用 LVGL 8，自定义多 Tab 页面与独立游戏屏幕，任务化刷新（`lvgl_task`）。
- **Wi-Fi 管理**：自动扫描、自动回连上一次网络、动态更新 UI 控件显示连接状态。
- **时间与天气**：通过 NTP 获取本地时间；调用心知天气接口展示当日与未来 7 日天气。
- **音乐播放**：从 SD 根目录扫描 `.mp3` 列表；支持背景音乐与游戏内切换；可调节音量与暂停。
- **语音交互**：实时语音识别（WebSocket 推送 PCM），获取文本后调用对话 API，再分段语音合成播报。
- **AI 聊天**：对接 MiniMax ChatCompletion（已硬编码示例，需自行替换安全密钥）；UI 展示问答区与语音开关。
- **小游戏集成**：通过独立目录与 LVGL 组件组织多款示例游戏与资源（图片、字体、地图）。
- **串口面板**：实时读取串口输入并追加显示在文本区域。

---

## 2. 硬件需求与引脚映射
| 功能 | 组件 | 主要引脚 | 说明 |
|------|------|---------|------|
| 显示屏 + 触摸 | TFT_eSPI + CST816S | 屏幕 SPI 依赖默认 TFT_eSPI 配置；触摸 SDA=3 SCL=18 RST=8 IRQ=46 | 当前分辨率 320x240，可在 `main.cpp` 修改 `screenWidth/Height` |
| 音频输出 (I2S) | DAC / 功放 | BCLK=40 LRC=41 DOUT=39 | `music.cpp` 中 `setPinout` 定义 |
| 麦克风 (I2S IN) | I2S MEMS MIC | SCK=11 WS=13 SD=12 | 采样率 16kHz，单声道 |
| SD 卡 (SPI) | Micro SD | CS=9 SCK=14 MISO=17 MOSI=10 | 频率 4MHz 可调整 |
| PSRAM | 板载 | - | 需使能 `-D BOARD_HAS_PSRAM` 以满足 LVGL + 音频缓存 |

---

## 3. 工程目录结构说明
```
src/
	main.cpp            // 入口：初始化 LVGL、任务创建、模块启动
	wifiuser.cpp/.h     // WiFi 扫描与连接逻辑 + UI 状态切换
	weather.cpp/.h      // NTP 与多日天气获取、JSON 解析与 UI 更新
	chatgpt.cpp/.h      // MiniMax ChatCompletion 调用示例
	mic2.cpp/.h         // 百度实时语音识别 + TTS 分段播报
	music.cpp/.h        // SD MP3 列表扫描与播放控制
	SDuser.cpp/.h       // SD 初始化与卡类型识别
	my_gui.cpp/.h       // 基础 GUI 示例与事件处理
	lvgl_games/         // 游戏入口 + 多款游戏资源与实现
		pvz/              // 植物大战僵尸
		lv_100ask_2048/   // 2048 游戏实现
		xiaoxiaole/       // 消消乐
		yang/             // 羊了个羊
ui/                   // UI 由 LVGL 代码生成器生成的组件、图片、字体、屏幕定义
include/, lib/        // 头文件与外部库占位
platformio.ini        // 构建与依赖配置
default_16MB.csv      // 分区表（自定义 16MB Flash）
```

---

## 4. 开发环境搭建
### 4.1 必备工具
- VS Code + PlatformIO 插件（推荐）或直接使用 `pio` CLI。
- 安装 USB 驱动（ESP32-S3 通常免驱，若识别异常请检查线材与端口）。

### 4.2 自动依赖安装
工程使用 `platformio.ini` 中的 `lib_deps` 自动拉取：
```
bblanchon/ArduinoJson@^6.21.4
lvgl/lvgl@^8.3.9
esphome/ESP32-audioI2S@^2.0.7
fbiego/CST816S@^1.1.1
plageoj/UrlEncode@^1.0.1
gilmaimon/ArduinoWebsockets@^0.5.3
bodmer/TFT_eSPI@2.5.0
```
首次编译会自动下载，无需手动添加。

### 4.3 构建步骤（PlatformIO）
1. 克隆仓库：
```powershell
git clone https://github.com/CaddonThaw/esp32s3-lvgl-terminal.git
cd esp32s3-lvgl-terminal
```
2. 打开 VS Code，确保 PlatformIO 已安装。
3. 连接开发板，选择环境 `env:esp32-s3-devkitc-1`。
4. 点击 “Build” 编译，通过后使用 “Upload” 烧录。
5. 打开 “Monitor” 查看串口调试输出（默认 115200）。

### 4.4 如果使用 CLI
```powershell
pio run
pio run --target upload
pio device monitor -b 115200
```

### 4.5 常见编译问题
- 若 LVGL 内存不足：确认已启用 PSRAM（默认宏已开启）。
- 若出现分区相关错误：保证使用仓库自带的 `default_16MB.csv`，与板子 Flash 容量匹配。

---

## 5. 烧录与打板注意事项
### 5.1 烧录
- ESP32-S3-DevKitC 默认 USB 下载，无需额外串口模块。
- 若无法进入下载：按住 `BOOT` 再点 `EN(RESET)`，然后放开 `BOOT`。
- 建议使用高质量 USB 数据线（排除供电不稳问题）。

### 5.2 原理图 / PCB / BOM 与下单
- hardware中提供完整的硬件资料：原理图、PCB 打板文件与 BOM 表（包含器件购买链接）。
- 在嘉立创（JLC）免费下单打板：
	- 直接使用仓库中提供的 PCB 文件；
	- 上传 GERBER 文件并选择常规参数（2 层、常规板厚、绿色油墨即可）；
	- 根据 BOM 在嘉立创下单或按链接采购元器件；

---

## 6. 运行时任务与逻辑概览
| 任务名 | 堆大小 | 核心 | 描述 |
|--------|--------|------|------|
| lvgl_task | 15KB | 1 | 定时调用 `lv_timer_handler()` 刷新界面 |
| WiFi_task | 5KB | 0 | 扫描 / 连接 / UI 状态切换 |
| serial_task | 5KB | 0 | 串口读取并追加到文本框 |
| time_task | 5KB | 0 | 每秒更新时间，按需触发天气刷新 |
| music_task | 5KB | 0 | 音频播放循环与模式切换 |
| mic_task | 5KB | 0 | 语音识别循环、状态机驱动 |

---

## 7. 声明
- 本项目参考了多个开源项目库，在此致谢各位大佬！
- 若发现问题或有改进建议，欢迎提交issue 与 PR ~
- 如果对您有帮助，欢迎在 GitHub 上给个 Star 支持一下，会持续更新功能与优化软件结构和硬件需求 ~
- 欢迎关注我的b站账号[会打狙的船长](https://space.bilibili.com/455474218?spm_id_from=333.40164.0.0)和抖音[嵌入式小黑子](https://www.douyin.com/user/MS4wLjABAAAA93EGipD_xAKDd-Qf6lBw07it01oe0HzHE5AW2j9ePjNjbGdEdMG7ic1TDM7EmsUI)，获取更多开源项目和嵌入式学习资料！

