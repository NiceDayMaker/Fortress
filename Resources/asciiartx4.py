import cv2
import numpy as np

# 유니코드 문자 마스크 정의
CHAR_MASKS = {
    '▀': np.array([[1, 1], [1, 1], [0, 0], [0, 0]], dtype=bool),
    '▖': np.array([[0, 0], [0, 0], [1, 0], [1, 0]], dtype=bool),
    '▗': np.array([[0, 0], [0, 0], [0, 1], [0, 1]], dtype=bool),
    '▘': np.array([[1, 0], [1, 0], [0, 0], [0, 0]], dtype=bool),
    '▝': np.array([[0, 1], [0, 1], [0, 0], [0, 0]], dtype=bool),
    '▌': np.array([[1, 0], [1, 0], [1, 0], [1, 0]], dtype=bool),
    '▚': np.array([[1, 0], [1, 0], [0, 1], [0, 1]], dtype=bool),
    '▂': np.array([[0, 0], [0, 0], [0, 0], [1, 1]], dtype=bool),
    '▆': np.array([[0, 0], [1, 1], [1, 1], [1, 1]], dtype=bool),
}

def best_fit_char(block):
    best_score = -float('inf')
    best_char, best_fg, best_bg = ' ', np.array([0, 0, 0]), np.array([0, 0, 0])
    for char, mask in CHAR_MASKS.items():
        fg_pixels = block[mask]
        bg_pixels = block[~mask]
        if len(fg_pixels) == 0 or len(bg_pixels) == 0:
            continue
        fg = np.median(fg_pixels, axis=0)
        bg = np.median(bg_pixels, axis=0)
        contrast = np.linalg.norm(fg - bg)
        score = contrast * 1.5 - np.var(fg_pixels) - np.var(bg_pixels)
        if score > best_score:
            best_score = score
            best_char, best_fg, best_bg = char, fg.astype(int), bg.astype(int)
    return best_char, best_fg, best_bg

def ansi_block(char, fg, bg):
    r1, g1, b1 = fg
    r2, g2, b2 = bg
    return f"\033[38;2;{r1};{g1};{b1}m\033[48;2;{r2};{g2};{b2}m{char}\033[0m"

def resize_image(image, new_width):
    height, width, _ = image.shape
    new_height = int(height * new_width / width)
    new_height += (4 - new_height % 4) % 4
    new_width += new_width % 2
    return cv2.resize(image, (new_width, new_height), interpolation=cv2.INTER_NEAREST)

def image_to_unicode_blocks_sharp(path, new_width=200):
    image = cv2.imread(path)
    if image is None:
        raise ValueError(f"Image at {path} could not be loaded.")
    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    image = resize_image(image, new_width)

    height, width, _ = image.shape
    result_pri = [[], [], []]  # [문자열, 전경색, 배경색]
    result = ""
    for y in range(0, height, 4):
        result_pri[0].append([])
        result_pri[1].append([])
        result_pri[2].append([])
        line = ""
        for x in range(0, width, 2):
            block = image[y:y+4, x:x+2]
            if block.shape[:2] != (4, 2):
                continue
            char, fg, bg = best_fit_char(block)
            result_pri[0][-1].append(char)
            result_pri[1][-1].append(fg.tolist())
            result_pri[2][-1].append(bg.tolist())
            line += ansi_block(char, fg, bg)
        result += line + "\n"
    return result, result_pri

# 실행 예시
if __name__ == "__main__":
    uploaded_path = "Resources/image.png"  # 이미지 경로를 본인 로컬 환경에 맞게 설정
    ascii_output_sharper, x4pri = image_to_unicode_blocks_sharp(uploaded_path, new_width=200)
    print(ascii_output_sharper)
    with open("Resources/asciix4.txt", "w", encoding="utf-8") as f:
        f.write(ascii_output_sharper)
