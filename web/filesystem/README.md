# Filesystem
A demonstration of folders as input provided by webkit in all major browsers.
The demo runs localy in a web browser via `demo.html`.

## 3 buttons

### Upload
this is where a folder selection is made.
on supported systems select the folder to open it in the application.
On less supported ststems selecting multiple files will almost do the same thing.

This will start loading files one by one at fixed intervals so the browser is less likely to crash.

### Feature
This is a pottentaly missconfigured option but it is intended to scroll automatically as files load in.

### Die
THis is the beloved kill switch.
If a mistake was made or the browser is hanging,
if you can hit the button in time,
it will destroy all the runtime copies,
kill any Timer loops and basically reset the application to new.

## Supported file types

- The program reads text files with any meme type aslong as it's text
- any image format supported by the webbrowser at the time this was written
- any video format same as images
- some object files may or maynot be supported
- everything else displays the file name as text so it's clear it cannot be opened

there's no reason non supported files can't be opened with some blob to dom function at a later date.
