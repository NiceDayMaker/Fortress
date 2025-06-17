import cv2
import os

def resize_image_double_rows(image, new_width=100):
    height, width, _ = image.shape
    new_height = int(height * new_width / width)
    new_height += new_height % 2  # 홀수면 1 더해서 짝수로
    return cv2.resize(image, (new_width, new_height), interpolation=cv2.INTER_LANCZOS4)

def color_block(top_rgb, bottom_rgb):
    r1, g1, b1 = top_rgb
    r2, g2, b2 = bottom_rgb
    return f"\033[38;2;{r1};{g1};{b1}m\033[48;2;{r2};{g2};{b2}m▀\033[0m"

def image_to_unicode_ascii(path, new_width=100):
    if not os.path.exists(path):
        raise FileNotFoundError(f"이미지 파일 경로가 잘못되었습니다: {path}")

    image = cv2.imread(path)
    if image is None:
        raise ValueError("이미지를 불러올 수 없습니다. 지원되지 않는 포맷일 수 있습니다.")

    image = resize_image_double_rows(image, new_width)
    rows = image.shape[0]

    result_pri = [[], [], []]  # [문자열, 전경색, 배경색]
    result = ""
    for y in range(0, rows - 1, 2):  # 마지막 줄 방지
        result_pri[0].append([])
        result_pri[1].append([])
        result_pri[2].append([])
        top_row = image[y]
        bottom_row = image[y + 1]
        for top_pixel, bottom_pixel in zip(top_row, bottom_row):
            b1, g1, r1 = top_pixel
            b2, g2, r2 = bottom_pixel
            result_pri[0][-1].append("▀")
            result_pri[1][-1].append([r1, g1, b1])
            result_pri[2][-1].append([r2, g2, b2])
            result += color_block((r1, g1, b1), (r2, g2, b2))
        result += "\n"
    return result, result_pri

# 사용 예
if __name__ == "__main__":
    path = "Resources/image.png"
    try:
        result, result_pri = image_to_unicode_ascii(path, new_width=100)
        print(result)
        with open("Resources/asciix2.txt", "w", encoding="utf-8") as f:
            f.write(result)
    except Exception as e:
        print("오류:", e)
