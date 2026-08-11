#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import logging
import sys
import time
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont

PROJECT_DIR = Path(__file__).resolve().parent.parent
PIC_DIR = PROJECT_DIR / "pic"
LIB_DIR = PROJECT_DIR / "lib"
sys.path.insert(0, str(LIB_DIR))

from lafvin_epd.epd2in9g import EPD

logging.basicConfig(level=logging.DEBUG)
logger = logging.getLogger(__name__)


def main():
    epd = EPD()
    try:
        logger.info("EPD_2IN9G Python demo")

        # 1. 初始化并清空屏幕
        logger.info("Initialize and clear")
        epd.init()
        epd.clear(epd.WHITE)

        font12 = ImageFont.truetype(str(PIC_DIR / "Font.ttc"), 12)
        font16 = ImageFont.truetype(str(PIC_DIR / "Font.ttc"), 16)

        # 2. 直接读取 BMP/JPG/PNG 等 Pillow 支持的图片
        logger.info("Display BMP image")
        epd.init_fast()
        with Image.open(PIC_DIR / "2in9g.bmp") as source_image:
            epd.display(epd.getbuffer(source_image))
        time.sleep(3)

        # 3. 使用 Pillow 绘图 API 创建画面
        logger.info("Draw shapes and text")
        epd.init()
        image = Image.new("RGB", (epd.height, epd.width), epd.WHITE)
        draw = ImageDraw.Draw(image)

        draw.point((10, 80), fill=epd.RED)
        draw.rectangle((10, 90, 13, 93), fill=epd.YELLOW)
        draw.rectangle((10, 100, 15, 105), fill=epd.BLACK)
        draw.line((20, 70, 70, 120), fill=epd.RED, width=1)
        draw.line((70, 70, 20, 120), fill=epd.RED, width=1)
        draw.rectangle((20, 70, 70, 120), outline=epd.YELLOW, width=1)
        draw.rectangle((80, 70, 130, 120), fill=epd.YELLOW)
        draw.ellipse((25, 75, 65, 115), outline=epd.BLACK, width=1)
        draw.ellipse((85, 75, 125, 115), fill=epd.BLACK)
        draw.line((85, 95, 125, 95), fill=epd.RED, width=1)
        draw.line((105, 75, 105, 115), fill=epd.YELLOW, width=1)

        draw.text((95, 0), "Four color e-Paper", font=font16, fill=epd.RED)
        draw.text((180, 20), "2.9 Inch e-Paper", font=font12, fill=epd.YELLOW)
        draw.text((160, 40), "你好世界", font=font16, fill=epd.BLACK)
        draw.text((210, 80), "Hello World", font=font16, fill=epd.BLACK)
        draw.text((250, 35), "123456", font=font12, fill=epd.RED)

        for x in range(5, 250, 10):
            draw.point((x, 50), fill=epd.RED)
        draw.rounded_rectangle(
            (5, 55, 220, 65), radius=5, fill=epd.RED, outline=epd.YELLOW, width=1
        )
        for index in range(8):
            x = 150 + index * 8
            draw.ellipse((x, 80, x + 5, 85), fill=epd.YELLOW)

        epd.display(epd.getbuffer(image))
        time.sleep(3)

        # 4. 清屏并进入深度休眠
        logger.info("Clear")
        epd.init()
        epd.clear(epd.WHITE)

        logger.info("Enter deep sleep")
        epd.sleep()
        return 0
    except KeyboardInterrupt:
        logger.info("Interrupted by user")
        return 130
    except Exception:
        logger.exception("Demo failed")
        raise
    finally:
        epd.close()


if __name__ == "__main__":
    raise SystemExit(main())
