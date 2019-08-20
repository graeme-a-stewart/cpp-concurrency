atom.keymaps.addKeystrokeResolver ({keystroke, event}) ->
  if event.code is 'Backspace' and (event.key is 'e' or event.key is 'E')
    keystroke.replace('backspace', 'e')
