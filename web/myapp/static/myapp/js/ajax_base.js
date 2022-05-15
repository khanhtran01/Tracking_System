$(document).ready(function () {
  $("#btn").click(function () {
    $.ajax({
      url: '/myapp',
      data: {
        'message': "Hello",
        'btn': 1
      },
      success: function (result) {
        console.log(result);
        $("#div1").html(result.data);
      }
    });
  });
});

$(document).ready(function () {
  $("form").submit(function (e) {
    e.preventDefault();
    var form = $(this);
    var actionUrl = form.attr('action');
    $.ajax({
      type: 'post',
      url: actionUrl,
      data: form.serialize(),
      success: function (result) {
        document.getElementById("input_id").value = '';
        console.log(result);
      }
    });
  });
});