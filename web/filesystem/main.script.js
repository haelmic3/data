"use strict";
/*
window.requestFileSystem
=	window.requestFileSystem
||	window.webkitRequestFileSystem
;
window.directoryEntry
=	window.directoryEntry
||	window.webkitDirectoryEntry
;
*/
const r={
	main:	document. getElementById("main"),
	input:	document. getElementById("file"),
	feature:document. getElementById("feature"),
}
var f={
	ready:false,
	list:[],
	iterator:{l:-1,k:0},
	timeout:-1,
	speed:100,
	delta:0,
}, process = function()
{
	if(f.timeout==-1)
	{f.timeout=setTimeout((()=>{
		if(f.ready)return ()=>
		{
			do
			{
				var cont = false;
				let file = f.list[f.iterator.l][f.iterator.k];
				if (!file){
					console. 
					log(	"Error in file processing:",
						{
							file:file,
							batch:f.iterator.l,
							item:f.iterator.k,
							size:f.list[
								f.iterator.l
							].length,
							list:f.list,
						},
					);
				}
				else if (file.type.includes("image\/"))
						//.match(/image\/.*/))
				r. main. insertAdjacentHTML
				(	'beforeend',
					'<img src="'
					+	URL.createObjectURL(file)
					+	'" alt="['
					+	file.type
					+	' '
					+	file.name
					+	'], "/>'
				);
				else if (file.type.includes("video\/"))
						//.match(/video\/.*/))
				r. main. insertAdjacentHTML
				(	'beforeend',
					'<video controls><source src="'
					+	URL.createObjectURL(file)
					+	'" type="'
					+	file.type
					+	'"/></video>'
				);
				else if
				(file.type==""||file.type.includes("text\/"))
				{
					let span = document
						.createElement("span");
					let att = document
						.createAttribute("class");
					att.value="item";
					span.setAttributeNode(att);
					r .main .appendChild(span);
					var reader=new Worker("file.worker.js");
					reader.onmessage=
					function(e){
						console.log("Reader response!");
						span.appendChild
						(
							document.createTextNode
							(
								e.data
							)
						);
					};
					reader.postMessage(file);
				}
				else
				{
					r. main. insertAdjacentHTML
					(	'beforeend',
						'<span class="item">['
						+	file.type
						+	' '
						+	file.name
						+	'], "</span>'
					);
					//cont = true;
				}
				if(f.iterator.k < f.list[f.iterator.l].length-1)
				{
					++f.iterator.k;
				}
				else if(f.iterator.l+1<f.list.length)
				{
					f.iterator.l++;
					f.iterator.k=0;
					
				}
				else
				{
					f.ready=false;
				}
			} while(f.ready && cont);
			f.timeout=-1;
			process();
		}
		else return ()=>
		{
			f.timeout=-1;
		};
	})(),(f.speed+=f.delta));}
};
r.input.addEventListener(
	"change",
	(e)=>{
		console.log("Input event.");
		e=e|window.event;
		let a = [];
		for(let i of r.input.files)a.push(i);
		f.list.push(a);
		if(!f.ready)
		{
			f.ready=true;
			f.iterator.l++;
			f.iterator.k=0;
			process();
		}
	}
);
var s = {
	limit:30,///TODO make user editable
	run:false,
	delta:0,
	speed:0,
	distance:0,
	time:Date.now(),
	frame:0,
	gap:
	{
		sum:0,
		at:	[],
	},
}
function autoscroll()
{
	s.frame=window.requestAnimationFrame(function(){autoscroll();});
	let	max
		=	document.documentElement.scrollTopMax,
		cur
		=	document.documentElement.scrollTop,
		dif
		=	max - cur,
		dd	//	The gap change [-15,-14,-17,185,0,...0,200...]
		=	dif - s.distance,
		td
		=	Date.now(),
		dt
		=	td - s.time,
		dt2
		=	dt * dt,
		a
		=	(dif/dt2-s.speed),
		j	// assumed target frame rate|a scaler of 1 pixel/sec
		=	15.0, // increase to slow scroll, non-neg non-zero
		n	// determines the history buffer size for the gap
		=	f.speed/4, // WARNING using non-associated global 
		dv
		=	(s.gap.sum)/(n*dt2)||0,
	_;
	if (0== s. frame%60)
	console. log
	({
		frame   :	s.frame,
		rate    :	(dt?parseInt(1000/dt):0)|0,
		time    :	dt,
		gap     :	s.gap.sum,
		speed   :	s.speed,
		delta   :	s.delta,
		loadV   :	dv,
		scrollA :	a,
		distance:	dif,
		span    :	max,
		n       :	n,
		limit   :	s.limit,
	});
	s. gap. sum += Math.abs((s. gap. at[s.frame%n]||0)-dd);
	s. gap. at[s.frame%n] = dd;
	/*if(0==s. frame%600)///MAGIC
	{
		s. n++;
	//	s.velocity.at.push(0);
	}*/
	s. time	= td;
	let sigdv = 1/(1+Math.pow(Math.E,-dv));
	let siga = 1/(1+Math.pow(Math.E,-a));
	s.delta	+= sigdv*dv+siga*a;
	if(s.delta=='Infinity')console.log(
		"Error!",{
		dv:dv,
		a:a,
		dt:dt,
		td:td,
		speed:s.speed,
		},
	);
	if (0 == s.gap.sum&& dv<1&& a<=0){s.speed=0;s.delta=0;}
	if (s.speed < s.delta) s.speed += 1/(1+Math.pow(Math.E,s.delta));
	if (s.speed > s.delta) s.speed -= 1/(1+Math.pow(Math.E,s.speed));
	if (s.speed>s.limit)s.speed=s.limit;
	s.distance = dif;
	document. documentElement. scrollBy(0, s. speed);
}
function auto()
{
	if(!s.run)
	{
		r.feature.innerHTML="On";
		s.run=true;
		s.time=Date.now();
		s.gap={sum:0,at:[],};
		s.frame=window.
			requestAnimationFrame(function(){autoscroll();});
	}
	else
	{
		s.run = false;
		r.feature.innerHTML="Off";
		window. cancelAnimationFrame(s.frame);
	}
}
document. body. addEventListener("keydown",(e)=>
{
	e=e||window.event;
	console.log(e);
	switch(e.keyCode)
	{
		case 65:
			auto();
	}
})
