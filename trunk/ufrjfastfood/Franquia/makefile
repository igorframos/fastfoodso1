all: 8

caixa_pagamento: caixa_pagamento.cpp
	g++ -o caixa_pagamento caixa_pagamento.cpp -c -lpthread

caixa_pedido: caixa_pedido.cpp
	g++ -o caixa_pedido caixa_pedido.cpp -c -lpthread

cliente: cliente.cpp
	g++ -o cliente cliente.cpp -c -lpthread

cozinheiro: cozinheiro.cpp
	g++ -o cozinheiro cozinheiro.cpp -c -lpthread

entregador: entregador.cpp
	g++ -o entregador entregador.cpp -c -lpthread

pedido: pedido.cpp
	g++ -o pedido pedido.cpp -c -lpthread

item: item.cpp
	g++ -o item item.cpp -c -lpthread

uff: uff.cpp
	g++ -o uff uff.cpp -lpthread

8: uff
	./uff

compile: caixa_pagamento caixa_pedido cliente cozinheiro entregador pedido item uff

clean:
	rm uff 
