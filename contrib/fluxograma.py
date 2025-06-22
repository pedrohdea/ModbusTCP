import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

# Etapas do fluxo simplificado
steps = [
    "Inicia o programa (PC)",
    "Seleciona opção 1 (Menu)",
    "Informa jogada (ex: D2D4)",
    "Converte jogada em binário",
    "Divide em resultadoA e resultadoB",
    "Monta quadro MODBUS TCP",
    "Envia quadro via Wi-Fi (porta 502)",
    "Inicia timeout",
    "ESP32 valida quadro",
    "Interpreta e imprime comando",
    "Envia resposta ao cliente",
    "Cliente valida resposta",
    "Se OK, exibe 'OK'",
    "Se erro, trata falha"
]

# Criar coordenadas do topo para baixo
positions = [(0, -i * 2) for i in range(len(steps))]

# Criar o gráfico
fig, ax = plt.subplots(figsize=(10, 18))
ax.set_xlim(-3, 3)
ax.set_ylim(-len(steps)*2, 2)
ax.axis('off')

# Adicionar caixas e setas corretamente (de cima para baixo)
for i, (text, (x, y)) in enumerate(zip(steps, positions)):
    # Caixa de texto
    box = mpatches.FancyBboxPatch((x - 1.5, y - 0.5), 3, 1,
                                  boxstyle="round,pad=0.1", edgecolor='black', facecolor='lightgray')
    ax.add_patch(box)
    ax.text(x, y, text, ha='center', va='center', fontsize=10)

    # Seta para o próximo bloco
    if i < len(steps) - 1:
        x_next, y_next = positions[i + 1]
        ax.annotate("", xy=(x_next, y_next + 0.5), xytext=(x, y - 0.5),
                    arrowprops=dict(arrowstyle="->", lw=1.5))


plt.tight_layout()
plt.show()
