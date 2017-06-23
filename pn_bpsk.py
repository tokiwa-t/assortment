import random
import math

MAX_ITARATION = 10000000
PI = 3.141592653589

n = int(input("Please input length of Shift-Register -> "))# シフトレジスタの段数を入力

print ("Input initial value\n") # シフトレジスタの初期化開始
sft_reg = 9*[0]
i = 0
for i in range(n):
	sft_reg[i] = int(input("{0}: Input initial value -> ".format(i+1))) # シフトレジスタの段数分初期値を入力する
# 初期化完了

i = 0
for i in range(n):
	print ("{0}: Initial value -> {1}".format(i+1,sft_reg[i])) # シフトレジスタの初期値を表示

fbk_reg = 5 * [0]
k = 0
while True:
	tmp = int(input("Input feedback register number \nTo exit input 0 -> ")) # 帰還タップを入力
	if tmp == 0:
		break
	fbk_reg[k] = tmp - 1
	k = k + 1
k = k - 1

# 符号長を計算
i = 0
len = int(1)
for i in range(n):
	len = len * 2
len = len - 1
len = len * 3

# シフトレジスタ動作
i = 0
pn_seq = 4096 * [-1]
for i in range(len):
	pn_seq[i] = sft_reg[n - 1]
	if k == 1:
		tmp = sft_reg[fbk_reg[0]] ^ sft_reg[fbk_reg[1]]
	if k == 3:
		tmp = sft_reg[fbk_reg[0]] ^ sft_reg[fbk_reg[1]] ^ sft_reg[fbk_reg[2]] ^ sft_reg[fbk_reg[3]]
	if k == 4:
		tmp = sft_reg[fbk_reg[0]] ^ sft_reg[fbk_reg[1]] ^ sft_reg[fbk_reg[2]] ^ sft_reg[fbk_reg[3]] ^ sft_reg[fbk_reg[4]]
	if k == 5:
		tmp = sft_reg[fbk_reg[0]] ^ sft_reg[fbk_reg[1]] ^ sft_reg[fbk_reg[2]] ^ sft_reg[fbk_reg[3]] ^ sft_reg[fbk_reg[4]] ^ sft_reg[fbk_reg[5]]
	j = n - 1
	while j > 0:
		sft_reg[j] = sft_reg[j - 1]
		j = j - 1
	sft_reg[0] = tmp

# 情報表示
print ("{0} number M system:".format(n))
i = 0
for i in range(k + 1):
	print("{0}".format(fbk_reg[i] + 1),)
print ("length: {0}".format(len/3))

i = 0
for i in range(len):
	print ("{0}".format(pn_seq[i]), end="")
	if (i + 1) % (len / 3) == 0 and i != 0:
		print ("\n", end="")

# 系列を0 → -1へ変換
i = 0
for i in range(len):
	if pn_seq[i] == 0:
		pn_seq[i] = -1

snr = int(input("Please input SNR[dB] -> "))

itr = 0
bit_count = 0
error_bit = 0
for itr in range(MAX_ITARATION):
	# PN系列よりビットの取出し, BPSKにつき1bit毎
	tx_bit1 = pn_seq[int(bit_count % (len / 3))]
	bit_count = bit_count + 1

	# BPSK変調
	tx_symbol_re = float(tx_bit1)
	tx_symbol_im = 0

	# ノイズ生成
	snrx = pow(10.0, (snr / 10.0))
	noise_var = 1.0 / snrx / 2.0

	xi = 1.0 - random.random()
	xq = 1.0 - random.random()

	ang = 2.0 * PI * xq;
	rad = math.sqrt(-2.0 * (math.log(xi)))

	# ノイズ
	noise_re = (math.sqrt(noise_var))*(rad*(math.cos(ang)))
	noise_im = (math.sqrt(noise_var))*(rad*(math.sin(ang)))

	# 受信信号へのノイズ加算
	rx_symbol_re = tx_symbol_re + noise_re
	rx_symbol_im = tx_symbol_im + noise_im

	# 復調
	if rx_symbol_re < 0.0:
		rx_bit1 = -1
	else:
		rx_bit1 = 1

	# エラーカウンタ
	if tx_bit1 != rx_bit1:
		error_bit = error_bit + 1

print("シミュレーション終了")
print("送信ビット数 = {0} 誤りビット数 = {1} BER {2}".format(bit_count, error_bit, float(error_bit / bit_count)))