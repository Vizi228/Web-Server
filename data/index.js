	const counter = document.querySelector('.count');
	const btn = document.querySelector('button');
	btn.addEventListener('click', () => {
		counter.innerText = +counter.innerText + 1;
	})