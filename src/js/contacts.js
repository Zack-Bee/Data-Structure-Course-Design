$(document).ready(function () {
    function init(data) {
        console.log(data);
        if (data !== "") {
            data = JSON.parse(data);
        }
    }

    var isFirstSignIn = false,
        $signBtn = $("#signBtn"),
        $phoneInput = $("#phoneInput"),
        $pswInput = $("#pswInput"),
        $btnGroup = $("#btnGroup"),
        $voiceBtn = $("#voiceBtn"),
        $newContactBtn = $("#newContactBtn"),
        $newGroupBtn = $("#newGroupBtn"),
        $progress = $("#progress"),
        account = {
            phone: "",
            contacts: null,
            groups: null
        };

    // 监听登录
    $signBtn.click(function () {
        $progress.removeClass("determinate");
        $progress.addClass("indeterminate");
        $signBtn.removeClass("pulse");
        account.phone = $phoneInput.val();
        var phoneNum = parseInt(account.phone),
            password = $pswInput.val();
        console.log(phoneNum);

        // 如果不是数字或者密码为空, 阻止登录
        if ((account.phone.length != 11) || isNaN(phoneNum) ||
            (phoneNum < 10000000000) || (password === "")) {
            Materialize.toast("请输入密码和11位的电话号码", 3000);
            $progress.removeClass("indeterminate");
            $progress.addClass("determinate");
            $signBtn.addClass("pulse");

            return;
        }

        // 向服务器发送账号密码, 并初始化页面
        $.post("../message", "init " + account.phone + " " + password + ";", function (data) {

        }, "json");
    })
})
