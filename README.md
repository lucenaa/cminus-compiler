# 🛠️ Diff Checker - Comparador de Saídas

## 📦 Estrutura do Projeto

```
├── killbuild           # Script para remover o diretório build

├── build/              # Diretório onde o projeto será compilado

├── output/             # Saídas esperadas (gabarito)

├── alunoout/           # Saídas geradas pelo código do aluno

├── example/            # Scripts de teste e exemplos

├── compare_diff.py     # Script que compara os arquivos e gera os diffs

└── CMakeLists.txt      # Configuração do CMake
```
## 🧪 Como usar
### 1. (Opcional) Limpar build anterior
```bash
./killbuild
```

### 2. Criar o build
```
mkdir -p build
cd build
cmake ..
make ddiff
cd ..
```

### 3. Rodar o comparador
```
python compare_diff.py
Isso irá gerar os arquivos de diff com base nas diferenças entre as saídas do aluno (alunoout/) e o gabarito (output/).
```

## 📁 Diretórios Importantes

### - alunoout/: Suas saídas geradas (o que seu código produziu)

### - output/: Gabarito oficial (o que era esperado)

### - example/: Scripts e testes prontos para rodar

## ✅ Requisitos
### Python 3.x

### CMake

### Compilador C/C++

