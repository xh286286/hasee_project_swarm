var debugMode = false,
m = MUSIC || {},
cs = {
    p: function() {
        try {
            window.console && console.log([].slice.call(arguments).join("\t"))
        } catch(e) {}
    }
};
var m_r_r_s = "";
function m_r_GetRUin() {
    var curMs;
    if (m_r_r_s.length > 0) return m_r_r_s;
    var u = getCookie("pgv_pvid");
    if ( !! u && u.length > 0) {
        m_r_r_s = u;
        return m_r_r_s
    }
    curMs = (new Date).getUTCMilliseconds();
    m_r_r_s = Math.round(Math.random() * 2147483647) * curMs % 1E10;
    document.cookie = "pgv_pvid=" + m_r_r_s + "; Expires=Sun, 18 Jan 2038 00:00:00 GMT; PATH=/; DOMAIN=qq.com;";
    return m_r_r_s
}
m.useIpv6 = 0;
m.isIpv6 = 0;
m.ipul = "http://ipv6.base.music.qq.com/fcgi-bin/fcg_ipv6_recognise.fcg?isjson=1";
m.expul = "http://base.music.qq.com/fcgi-bin/fcg_musicexpress.fcg?json=3&guid=" + m_r_GetRUin();
m.expType = 2;
m.playerArr = [{
    3 : 100
},
{
    4 : 100
},
{
    5 : 100
},
{
    6 : 100
},
{
    7 : 100
}];
top.g_JData = top.g_JData || {};
function loadJsonData(x, u, cb, ecb, r, c, f) {
    var s, d = top.g_JData;
    if (d[x] && (!r && !d[x].error)) {
        cb(d[x]);
        return
    }
    c = c || "GB2312";
    f = f || "JsonCallback";
    s = new MUSIC.JSONGetter(u, void 0, null, c);
    s.onSuccess = function(o) {
        try {
            cb(d[x] = o)
        } catch(e) {}
    };
    ecb && (s.onError = ecb);
    s.send(f)
}
m.OZ = {
    baseSpeed: 0,
    pingSender: function(url) { (new Image).src = url
    },
    speedSet: function(f2, f3, id, value, basetime, baseSpeed) {
        var key = f2 + "_" + f3,
        startTime, endTime, speed;
        baseSpeed = baseSpeed || m.OZ.baseSpeed; ! this[key] && (this[key] = []);
        startTime = basetime || this[key][0];
        endTime = value || +new Date;
        id !== 0 && (startTime !== undefined && startTime !== null) && (speed = endTime - startTime);
        id === 0 && (speed = endTime - 1 + 1);
        speed !== undefined && speed > baseSpeed && (this[key][id] = speed);
        cs.p("speedSet,flag1=170,flag2=", f2, ",flag3=", f3, ",id=", id, ",speed=", speed)
    },
    speedSend: function(f2, f3, rate) {
        rate = rate || 1;
        if (! (typeof rate == "number" && (rate > 0 && +new Date % rate == 0))) return;
        var key = f2 + "_" + f3,
        data = this[key],
        _arr = [],
        url;
        if (!data || data.length <= 0) return false;
        for (var i = 1; i < data.length; i++) data[i] && _arr.push(i + "=" + data[i]);
        url = "http://isdspeed.qq.com/cgi-bin/r.cgi?flag1=170&flag2=" + f2 + "&flag3=" + f3 + "&" + _arr.join("&");
        cs.p("speed report:", url, ";_arr.length:", _arr.length, ";data.length:", data.length);
        _arr.length > 0 && this.pingSender(url);
        this[key] = []
    },
    build_args: function(d) {
        var buf = [];
        for (var n in d) buf.push(n + "=" + d[n]);
        return buf.join("&")
    },
    errorSend: function(f2, f3, data) {
        if (!MUSIC.lang.isHashMap(data)) return 0;
        url = "http://isdspeed.qq.com/cgi-bin/r.cgi?flag1=170&flag2=" + f2 + "&flag3=" + f3 + "&" + this.build_args(data);
        cs.p("errorSend:", url);
        this.pingSender(url)
    }
};
MUSIC.module.webPlayer = {};
MUSIC.module.webPlayer.playStatus = {
    S_UNDEFINE: 0,
    S_STOP: 1,
    S_PAUSE: 2,
    S_PLAYING: 3,
    S_BUFFERING: 4,
    S_PLAYBEGIN: 5,
    S_PLAYEND: 6
};
MUSIC.module.webPlayer.interFace = function() {
    var VQQPlayer = null;
    var MediaPlayer = null;
    var VH5Player = null;
    var VFlashPlayer = null;
    var webPlayer = null;
    var playerList = null;
    var musicInitReady = false;
    var isLoading = false;
    var songDuration = 0;
    var curPostion = 0;
    var mIsLoop = false;
    var mIsH5Mp3 = true;
    var mPlayerType = 0;
    var mOption = {
        fromtag: 29,
        statFromtag: 0,
        errorTips: function(title, text) {
            alert(title + "</br>" + text)
        }
    };
    var wmaurl_tpl = "http://stream%(stream).qqmusic.qq.com/%(sid).mp3";
    var wmaurl_tpl2 = "http://stream%(stream).qqmusic.qq.com/%(sid).wma";
    var mp3url_tpl = "http://stream%(stream).qqmusic.qq.com/%(sid).mp3";
    var tpturl_tpl = "http://tpt.music.qq.com/%(sid).tpt";
    var songInfoObj = {
        mstream: 0,
        murl: "",
        msong: "",
        msinger: "",
        mQzoneKey: "",
        mid: 0,
        mSongType: 0
    };
    function init(opt) {
        MUSIC.object.extend(mOption, opt || {})
    }
    function getOption() {
        return mOption
    }
    function getWebPlayer() {
        return webPlayer
    }
    function setCurPostion(cp, duration) {
        curPostion = cp;
        songDuration = duration
    }
    function getCurPostion() {
        return curPostion
    }
    function getSongDuration() {
        return songDuration
    }
    function getPostion() {
        return playerList.getPostion()
    }
    function initSongInfoObj() {
        songInfoObj = {
            mstream: 0,
            murl: "",
            msong: "",
            msinger: "",
            mQzoneKey: "",
            mid: 0,
            mSongType: 0
        }
    }
    function setSongInfoObj(obj) {
        initSongInfoObj();
        MUSIC.object.extend(songInfoObj, obj || {});
        g_playerStat.add(songInfoObj)
    }
    function getSongInfoObj() {
        return songInfoObj
    }
    function loadWmPlayer(callback) {
        var jsloader = new MUSIC.JsLoader;
        jsloader.onload = function() {
            try {
                MediaPlayer = g_player.wmPlayer(mOption.fromtag);
                MediaPlayer.createActiveX();
                MediaPlayer.initialize()
            } catch(e) {
                if (/win/.test(navigator.platform.toLowerCase()) && (( !! ua.firefox || !!ua.ie) && typeof g_dialog != "undefined")) g_dialog.show({
                    mode: "common",
                    title: "\u60a8\u4f7f\u7528\u7684\u662f\u65e7\u7248\u97f3\u4e50\u63a7\u4ef6\uff0c\u8bf7\u8fdb\u884c\u66f4\u65b0\u3002\u6216\u68c0\u67e5\u6d4f\u89c8\u5668\u662f\u5426\u7981\u7528\u4e86\u63a7\u4ef6\u3002",
                    icon_type: 1,
                    button_info1: {
                        highlight: 1,
                        onclick: "window.open('http://dl_dir.qq.com/music/clntupate/QzoneMusicInstall.exe');g_dialog.hide();",
                        title: "\u7acb\u5373\u66f4\u65b0"
                    },
                    button_info2: {
                        highlight: 0,
                        onclick: "g_dialog.hide();",
                        title: "\u5173\u95ed"
                    }
                });
                else mOption.errorTips("\u64ad\u653e\u6b4c\u66f2\u5931\u8d25\uff01", "\u60a8\u6ca1\u6709\u5b89\u88c5WindowsMediaPlayer\u63d2\u4ef6\u6216\u8be5\u63d2\u4ef6\u88ab\u7981\u7528\u3002");
                return false
            }
            musicInitReady = true;
            isLoading = false;
            webPlayer = MediaPlayer;
            mPlayerType = 2;
            EventUtil(window, "unload", g_playerCallback.OnUnitialized);
            if (callback) callback()
        };
        jsloader.onerror = function() {
            mOption.errorTips("\u64ad\u653e\u6b4c\u66f2\u5931\u8d25\uff01", "\u52a0\u8f7dwmp\u64ad\u653e\u63a7\u4ef6\u5931\u8d25\uff0c\u8bf7\u7a0d\u540e\u91cd\u8bd5\u3002")
        };
        jsloader.load("http://imgcache.gtimg.cn/music/portal_v3/js/wmp.js", null, "utf-8")
    }
    function loadFlashPlayer(callback) {
        var jsloader = new MUSIC.JsLoader;
        jsloader.onload = function() {
            try {
                VFlashPlayer = g_player.qFlash(mOption.fromtag);
                VFlashPlayer.createActiveX();
                VFlashPlayer.initialize()
            } catch(e) {
                alert("exception:" + e.message)
            }
            musicInitReady = true;
            isLoading = false;
            webPlayer = VFlashPlayer;
            mPlayerType = 4;
            EventUtil(window, "unload", g_playerCallback.OnUnitialized);
            if (callback) callback()
        };
        jsloader.onerror = function() {
            mOption.errorTips("\u64ad\u653e\u6b4c\u66f2\u5931\u8d25\uff01", "\u52a0\u8f7dFlash\u64ad\u653e\u63a7\u4ef6\u5931\u8d25\uff0c\u8bf7\u7a0d\u540e\u91cd\u8bd5\u3002")
        };
        jsloader.load("http://imgcache.gtimg.cn/music/portal_v3/js/flashplayer.js", null, "utf-8")
    }
    function loadH5AudioPlayer(callback) {
        MUSIC.useIpv6 = 1;
        var jsloader = new MUSIC.JsLoader;
        jsloader.onload = function() {
            try {
                VH5Player = g_player.h5Audio(mOption.fromtag);
                VH5Player.createActiveX();
                VH5Player.initialize()
            } catch(e) {
                alert("exception:" + e.message)
            }
            musicInitReady = true;
            isLoading = false;
            webPlayer = VH5Player;
            mPlayerType = 3;
            var audio = document.createElement("audio");
            if ( !! audio.canPlayType);
            else {
                mIsH5Mp3 = false;
                mOption.errorTips("\u64ad\u653e\u6b4c\u66f2\u5931\u8d25\uff01", "\u8be5\u529f\u80fd\u76ee\u524d\u4e0d\u652f\u6301\u60a8\u7684\u6d4f\u89c8\u5668\uff0c\u8bf7\u4f7f\u7528chrome\uff0c\u9ad8\u7248\u672csafari\uff0cfirefox\u6216\u8005IE\u8fdb\u884c\u64ad\u653e\u3002")
            }
            EventUtil(window, "unload", g_playerCallback.OnUnitialized);
            if (callback) callback()
        };
        jsloader.onerror = function() {
            mOption.errorTips("\u64ad\u653e\u6b4c\u66f2\u5931\u8d25\uff01", "\u52a0\u8f7dhtml5 audio\u5931\u8d25\uff0c\u8bf7\u7a0d\u540e\u91cd\u8bd5\u3002")
        };
        jsloader.load("http://imgcache.gtimg.cn/music/portal_v3/js/h5audio.js", null, "utf-8")
    }
    function initMusic(callback) {
        var _music = g_webPlayer.getSongInfoObj() || {
            msongurl: ""
        };
        if ( !! ua.ie && /share\.weiyun\.qq\.com/i.test(_music.msongurl)) ! VFlashPlayer && (g_webPlayer.stopPlayer(), VH5Player = null, musicInitReady = false);
        if (!musicInitReady) {
            if (!isLoading);
            isLoading = true;
            EventUtil(window, "unload",
            function() {
                g_playerStat.add()
            });
            var _audio = document.createElement("audio"),
            _h5Support = !!(_audio && _audio.canPlayType);
            if (_h5Support && !( !! ua.ie && /share\.weiyun\.qq\.com/i.test(_music.msongurl))) loadH5AudioPlayer(callback);
            else loadFlashPlayer(callback)
        } else if (callback) callback()
    }
    function OnSongPlayBegin(songinfo, index, total) {}
    function OnSongPlayEnd(songinfo, index, total) {}
    function OnSongPlaying(lCurPos, lTotal) {}
    function OnSongDownloading(progress) {}
    function OnPlayPause() {}
    function OnPlayStop() {}
    function OnPlaying() {}
    function OnPlayError(songinfo, index) {
        cs.p("g_webPlayer.OnPlayError 1");
        if (isQusicSong(songinfo)) {
            cs.p("g_webPlayer.OnPlayError 2");
            MUSIC.OZ.errorSend(118, 4, {
                8 : 100
            });
            cs.p("g_webPlayer.OnPlayError 3");
            songinfo.mcdn = "ws";
            cs.p("g_webPlayer.OnPlayError 4");
            webPlayer.setPlayURL();
            cs.p("g_webPlayer.OnPlayError 5")
        } else mOption.errorTips("\u6b4c\u66f2\u64ad\u653e\u5931\u8d25\uff01", "\u60a8\u6dfb\u52a0\u7684\u7f51\u7edc\u6b4c\u66f2\uff0c\u5730\u5740\u51fa\u9519\u6216\u88ab\u4e3b\u4eba\u5220\u9664\u3002")
    }
    function startPlayer() {
        if (!playerList || playerList.getCount() <= 0) return false; !! webPlayer && webPlayer.startPlayer()
    }
    function pausePlayer() { !! webPlayer && webPlayer.pausePlayer()
    }
    function stopPlayer() { !! webPlayer && webPlayer.stopPlayer()
    }
    function lastPlayer() {
        if (!playerList || playerList.getCount() <= 0) return false;
        playerList.lastPostion();
        playList();
        return true
    }
    function nextPlayer() {
        if (!playerList || playerList.getCount() <= 0) return false;
        g_webPlayer.OnSongPlayEnd(getSongInfoObj(), playerList.getPostion(), playerList.getCount());
        playerList.nextPostion();
        playList();
        return true
    }
    function autoNextPlayer() {
        if (!playerList || playerList.getCount() <= 0) return false;
        g_webPlayer.OnSongPlayEnd(getSongInfoObj(), playerList.getPostion(), playerList.getCount());
        if (playerList.autoNextPostion()) playList();
        else stopPlayer();
        return true
    }
    function playAnyPos(pos) {
        if (!playerList || playerList.getCount() <= 0) return false;
        playerList.setPostion(pos);
        playList();
        return true
    }
    function addSong(list, isPlay) {
        var pos = playerList.getCount();
        playerList.addSongList(list);
        if (isPlay) {
            playerList.setPostion(pos);
            playList()
        }
    }
    function delSong(pos) {
        var curPos = playerList.getPostion();
        playerList.delSong(pos);
        if (pos == curPos) {
            if (playerList.getCount() <= 0) {
                stopPlayer();
                return false
            }
            if (curPos >= playerList.getCount() - 1) playerList.setPostion(playerList.getCount() - 1);
            playList()
        }
        return true
    }
    function insertSong(pos, songinfo) {
        if (!playerList) return false;
        playerList.insertSong(pos, songinfo);
        return true
    }
    function mutePlayer() { !! webPlayer && webPlayer.setMute()
    }
    function getVolumn() {
        if (!webPlayer) return 0;
        return webPlayer.getVolumn()
    }
    function setVolumn(vol) { !! webPlayer && webPlayer.setVolumn(vol)
    }
    function setPlayerState(status) { !! webPlayer && webPlayer.setPlayerState(status)
    }
    function setPlayProgress(curtime) { !! webPlayer && webPlayer.setPlayProgress(curtime)
    }
    function setDownloadProgress(progress) { !! webPlayer && webPlayer.setDownloadProgress(progress)
    }
    function getDownloadProgress() {
        if (!webPlayer) return 0;
        return webPlayer.getDownloadProgress()
    }
    function playSong(obj) {
        if (typeof obj != "object") {
            mOption.errorTips("\u6b4c\u66f2\u4fe1\u606f\u9519\u8bef\uff01", "");
            return
        }
        if (!obj.mstream || !obj.mid) {
            mOption.errorTips("\u6b4c\u66f2\u4fe1\u606f\u9519\u8bef\uff01", "");
            return
        }
        setSongInfoObj(obj);
        initMusic(function() {
            webPlayer.setPlayURL()
        })
    }
    function setPlayerList(isPlay, arr, mode) { !! playerList || (playerList = g_playerList());
        playerList.setPlayerList(arr, mode);
        if (isPlay) nextPlayer()
    }
    function setMode(mode) {
        if (!playerList) return false;
        playerList.setMode(mode);
        return true
    }
    function playList() { !! playerList || (playerList = g_playerList());
        setSongInfoObj(playerList.getSongInfoObj());
        playBegin();
        if (! ( !! ua.isiPad || !!ua.isiPhone)) setTimeout(function() {
            initMusic(function() {
                webPlayer.setPlayURL()
            })
        },
        0);
        else initMusic(function() {
            webPlayer.setPlayURL()
        })
    }
    function playBegin() {
        if (!playerList) return;
        g_webPlayer.OnSongPlayBegin(getSongInfoObj(), playerList.getPostion(), playerList.getCount())
    }
    function getPlayerSource() {
        if ( !! VQQPlayer) return VQQPlayer.getPlayerSource()
    }
    function getCurrentPlayerSource() {
        if ( !! VQQPlayer) return VQQPlayer.getCurrentPlayerSource()
    }
    function setCurrentPlayerSource(args) { !! VQQPlayer && VQQPlayer.setCurrentPlayerSource(args)
    }
    function clearPlayerList() {
        if (!playerList) return;
        playerList.clearPlayerList();
        stopPlayer()
    }
    function isQusicSong(songobj) {
        return MUSIC.lang.isHashMap(songobj) && ( !! songobj.mid && ( !! songobj.mstream && (songobj.mid > 0 && songobj.mstream > 0)))
    }
    function getPlayUrl(songobj, callback, option) {
        if (MUSIC.lang.isHashMap(callback)) {
            option = callback;
            callback = undefined
        }
        option = option || {
            cdn: "cc"
        };
        option.cdn = option.cdn || "cc";
        if (songobj.mcdn == "ws") {
            option.cdn = "ws";
            songobj.mcdn = "cc"
        }
        var url = "",
        sid = 0,
        mp3_mid_tpl = {
            48 : "C200%(mid)",
            128 : "M500%(mid)",
            320 : "M800%(mid)"
        },
        rate = "128",
        mid = "",
        expPlayUL = function() {
            var d = top.g_JData["express"],
            t = +new Date,
            ipv6PlayUL = function() {
                var data = top.g_JData["ipver"];
                m.getIpVer = 1;
                if (!MUSIC.lang.isHashMap(data) || (data.code != 0 || !data.ipinfo)) {
                    m.isIpv6 = 0;
                    cs.p("ipv6PlayUL 1,m.isIpv6:0")
                } else m.isIpv6 = data.ipinfo == "ipv6";
                if (m.useIpv6 && (m.isIpv6 && (MUSIC.lang.isHashMap(d) && d.code == 0))) {
                    m.expType = 9;
                    url = "http://ipv6.base.music.qq.com/" + mid + ".mp3?vkey=" + d.key + "&guid=" + m_r_GetRUin()
                }
                cs.p("ipv6PlayUL 2,m.isIpv6:", m.isIpv6, ";url:", url);
                callback(songobj.playUrl = url)
            };
            try {
                m.OZ.errorSend(118, 4, m.playerArr[mPlayerType]);
                m.OZ.speedSet(118, 4, 0, t);
                m.getExp = 1;
                if (!MUSIC.lang.isHashMap(d) || (d.code != 0 || !d.key)) {
                    cs.p("expPlayUL 2,url:" + url);
                    m.useExp = 0;
                    m.expType = 2
                } else {
                    m.expType = option.cdn == "cc" ? 1 : 2;
                    m.useExp = option.cdn == "cc" ? 1 : 0;
                    url = "http://" + option.cdn + ".stream.qqmusic.qq.com/" + mid + ".m4a?vkey=" + d.key + "&guid=" + m_r_GetRUin() + "&fromtag=0";
                    cs.p("expPlayUL 3,url:", url)
                }
                if (m.useIpv6 && option.cdn == "cc") {
                    cs.p("m.getIpVer 0");
                    if (!m.getIpVer) setTimeout(function() {
                        loadJsonData("ipver", m.ipul, ipv6PlayUL, ipv6PlayUL, 1, "GB2312", "MusicJsonCallback")
                    },
                    0);
                    else ipv6PlayUL()
                } else callback(songobj.playUrl = url);
                if (!m.tExp) {
                    m.tExp = 1;
                    setTimeout(function() {
                        m.getExp = 0;
                        m.tExp = 0
                    },
                    72E5)
                }
            } catch(e) {
                cs.p("expPlayUL exp:" + e.message)
            }
            return url
        };
        if (isQusicSong(songobj)) {
            sid = parseInt(songobj.mid) + 3E7;
            rate = songobj.mrate ? songobj.mrate > 1E3 ? songobj.mrate / 1E3: songobj.mrate: songobj.size320 > 0 ? "320": "128";
            mid = songobj.mmid ? mp3_mid_tpl["48"].jstpl_format({
                mid: songobj.mmid
            }) : sid;
            url = wmaurl_tpl.jstpl_format({
                stream: parseInt(songobj.mstream) + 10,
                sid: sid
            });
            cs.p("getSongUrl 1,url:" + url);
            if (callback) if (!m.getExp) {
                cs.p("m.getExp 0");
                loadJsonData("express", m.expul, expPlayUL, expPlayUL, 1, "GB2312", "jsonCallback")
            } else {
                cs.p("m.getExp 1");
                expPlayUL()
            }
        } else if ( !! songobj.msongurl) {
            url = songobj.msongurl;
            callback && callback(url)
        }
        cs.p("getSongUrl 2,url:" + url);
        return url
    }
    function getSongUrl(songobj, callback) {
        return getPlayUrl(songobj, callback, {
            cdn: "cc"
        })
    }
    function getWsUrl(songobj, callback) {
        return getPlayUrl(songobj, callback, {
            cdn: "ws"
        })
    }
    function getTptUrl(songobj) {
        var url = "";
        if (isQusicSong(songobj)) {
            var sid = parseInt(songobj.mid) + 3E7;
            url = tpturl_tpl.jstpl_format({
                sid: sid
            })
        }
        return url
    }
    function isSupportMp3() {
        return mIsH5Mp3
    }
    function getPlayerType() {
        return mPlayerType
    }
    return {
        wmaurl_tpl: wmaurl_tpl,
        wmaurl_tpl2: wmaurl_tpl2,
        mp3url_tpl: mp3url_tpl,
        tpturl_tpl: tpturl_tpl,
        setSongInfoObj: setSongInfoObj,
        getSongInfoObj: getSongInfoObj,
        getWebPlayer: getWebPlayer,
        initMusic: initMusic,
        startPlayer: startPlayer,
        pausePlayer: pausePlayer,
        stopPlayer: stopPlayer,
        lastPlayer: lastPlayer,
        nextPlayer: nextPlayer,
        autoNextPlayer: autoNextPlayer,
        playAnyPos: playAnyPos,
        addSong: addSong,
        delSong: delSong,
        insertSong: insertSong,
        mutePlayer: mutePlayer,
        getVolumn: getVolumn,
        setVolumn: setVolumn,
        setPlayProgress: setPlayProgress,
        setDownloadProgress: setDownloadProgress,
        getDownloadProgress: getDownloadProgress,
        playSong: playSong,
        setPlayerState: setPlayerState,
        setCurPostion: setCurPostion,
        getCurPostion: getCurPostion,
        getSongDuration: getSongDuration,
        getPostion: getPostion,
        setPlayerList: setPlayerList,
        playList: playList,
        OnSongPlayBegin: OnSongPlayBegin,
        OnSongPlayEnd: OnSongPlayEnd,
        OnSongPlaying: OnSongPlaying,
        OnPlayPause: OnPlayPause,
        OnPlayStop: OnPlayStop,
        OnPlayError: OnPlayError,
        OnSongDownloading: OnSongDownloading,
        playBegin: playBegin,
        OnPlaying: OnPlaying,
        getPlayerSource: getPlayerSource,
        getCurrentPlayerSource: getCurrentPlayerSource,
        setCurrentPlayerSource: setCurrentPlayerSource,
        setMode: setMode,
        clearPlayerList: clearPlayerList,
        isQusicSong: isQusicSong,
        getSongUrl: getSongUrl,
        getWsUrl: getWsUrl,
        getTptUrl: getTptUrl,
        isSupportMp3: isSupportMp3,
        getPlayerType: getPlayerType,
        init: init,
        getOption: getOption
    }
} ();
MUSIC.module.webPlayer.playerList = function() {
    var mPostion = -1;
    var mMode = 1;
    var mpList = [];
    function getCount() {
        return mpList.length
    }
    function lastPostion() {
        mPostion = (mPostion - 1 + mpList.length) % mpList.length;
        return mPostion
    }
    function nextPostion() {
        if (mMode == 4) {
            var rnd = parseInt(Math.random() * 1E5) % getCount();
            if (rnd == mPostion) rnd = (rnd + 1) % getCount();
            mPostion = rnd
        } else mPostion = (mPostion + 1) % getCount();
        return mPostion
    }
    function autoNextPostion() {
        if (mMode == 1) {
            if (mPostion < 0 || mPostion >= getCount()) mPostion = 0
        } else if (mMode == 2) {
            if (isLastPlayer()) return false;
            mPostion = (mPostion + 1) % getCount()
        } else if (mMode == 3) mPostion = (mPostion + 1) % getCount();
        else if (mMode == 4) {
            var rnd = parseInt(Math.random() * 1E5) % getCount();
            if (rnd == mPostion) rnd = (rnd + 1) % getCount();
            mPostion = rnd
        }
        return true
    }
    function setMode(mode) {
        if (mode < 1 || mode > 5) mode = 1;
        mMode = mode
    }
    function setPostion(pos) {
        if (pos >= 0 && pos < mpList.length) mPostion = pos
    }
    function getPostion() {
        return mPostion
    }
    function isLastPlayer() {
        return mPostion + 1 == mpList.length
    }
    function getSongInfoObj() {
        return mpList[mPostion]
    }
    function setPlayerList(arr, mode) {
        if (typeof arr != "object") return false;
        clearPlayerList();
        for (var i = 0,
        len = arr.length; i < len; i++) if (typeof arr[i] == "object") mpList.push(arr[i]);
        mPostion = -1;
        if (typeof mode == "undefined") setMode(2);
        else setMode(mode)
    }
    function addSongList(list) {
        for (var i = 0,
        len = list.length; i < len; i++) if (typeof list[i] == "object") mpList.push(list[i])
    }
    function delSong(pos) {
        if (pos >= 0 && pos < mpList.length) mpList.splice(pos, 1);
        if (pos < mPostion) mPostion--;
        if (mPostion >= mpList.length) mPostion = mpList.length - 1;
        if (mpList.length == 0) mPostion = -1
    }
    function insertSong(pos, songinfo) {
        if (pos >= 0 && pos < mpList.length) mpList.splice(pos, 0, songinfo);
        if (pos <= mPostion) mPostion++
    }
    function clearPlayerList() {
        for (var i = 0,
        len = mpList.length; i < len; i++) delete mpList[i];
        mpList = [];
        mPostion = -1
    }
    return {
        getCount: getCount,
        isLastPlayer: isLastPlayer,
        lastPostion: lastPostion,
        nextPostion: nextPostion,
        autoNextPostion: autoNextPostion,
        setPostion: setPostion,
        getPostion: getPostion,
        getSongInfoObj: getSongInfoObj,
        setPlayerList: setPlayerList,
        addSongList: addSongList,
        delSong: delSong,
        clearPlayerList: clearPlayerList,
        setMode: setMode,
        insertSong: insertSong
    }
};
MUSIC.module.webPlayer.qqPlayer = function(fromTag) {
    var $T = top,
    $ = MUSIC,
    $D = $.dom,
    $E = $.event;
    var mQQPlayerConfig = {
        REP_PLAYURL_IP_ARRAY: ["121.14.73.62", "121.14.73.48", "58.60.9.178", "58.61.165.54"],
        REP_PLAYURL_PORT: 17785,
        P2P_UDP_SVR_IP_ARRAY: ["119.147.65.30", "58.61.166.180", "pdlmusic.p2p.qq.com"],
        P2P_UDP_SVR_PORT: 8E3,
        P2P_TCP_SVR_IP_ARRAY: ["119.147.65.30", "58.61.166.180", "pdlmusic.p2p.qq.com"],
        P2P_TCP_SVR_PORT: 433,
        P2P_STUN_SVR_IP: "stun-a1.qq.com",
        P2P_STUN_SVR_PORT: 8E3,
        P2P_TORRENT_URL: "http://219.134.128.55/",
        P2P_CACHE_SPACE: 100,
        STAT_REPORT_SVR_IP: "219.134.128.41",
        STAT_REPORT_SVR_PORT: 17653,
        REP_PLAYSONG_IP_ARRAY: ["58.60.11.85", "121.14.96.113", "58.61.165.50", "121.14.95.82"],
        REP_PLAYSONG_PORT: 8E3,
        REP_SONGLIST_IP_ARRAY: ["121.14.94.181", "121.14.94.183"],
        REP_SONGLIST_PORT: 8E3
    };
    var mPlayerSource = "";
    var mCurPlaySrc = "";
    var mPlayerState = g_playerStatus.S_UNDEFINE;
    var mPlayerName = "";
    var mUinCookie = 12345678;
    var mKeyCookie = "12345678";
    var mFromTag = fromTag;
    var mIsInit = false;
    var plv = "0";
    var playerSrcSeted = false;
    var lastBufTime = 0;
    var mDownloadProgress = 0;
    function setPlayList() {}
    function resetCache() {}
    function setPlayParams(iMusicId, sul) {
        mPlayerName.SetCookie("qqmusic_uin", mUinCookie);
        mPlayerName.SetCookie("qqmusic_key", mKeyCookie);
        mPlayerName.SetCookie("qqmusic_fromtag", mFromTag);
        var tiMusicId = "" + iMusicId;
        mPlayerName.SetCookie("qqmusic_musicid", tiMusicId);
        mPlayerName.SetCookie("qqmusicchkkey_key", mKeyCookie);
        mPlayerName.SetCookie("qqmusicchkkey_url", sul)
    }
    function setPlayerVersion(version) {
        plv = version
    }
    function getPlayerSource() {
        return mPlayerSource
    }
    function getCurrentPlayerSource() {
        return mCurPlaySrc
    }
    function setCurrentPlayerSource(args) {
        mCurPlaySrc = args
    }
    function insertQQPlayer(args) {
        var isIE = window.ActiveXObject ? true: false;
        if (isIE) {
            var params = {};
            var objAttrs = {};
            for (var k in args) switch (k) {
            case "classid":
            case "style":
            case "name":
            case "height":
            case "width":
            case "id":
                objAttrs[k] = args[k];
                break;
            default:
                params[k] = args[k]
            }
            var str = [];
            str.push("<object ");
            for (var i in objAttrs) {
                str.push(i);
                str.push('="');
                str.push(objAttrs[i]);
                str.push('" ')
            }
            str.push(">");
            for (var i in params) {
                str.push('<param name="');
                str.push(i);
                str.push('" value="');
                str.push(params[i]);
                str.push('" /> ')
            }
            str.push("</object>");
            var playerDiv = $D.createElementIn("div", "musicproxy");
            playerDiv.innerHTML = str.join("");
            return playerDiv.firstChild
        } else {
            var playerDiv = $D.createElementIn("div", "musicproxy");
            playerDiv.style.cssText = "height:0px;overflow:hidden";
            playerDiv.innerHTML = '<embed id="QzoneMusic" type="application/tecent-qzonemusic-plugin" width="0px" height="0px" />';
            var QzonePlayer = document.getElementById("QzoneMusic");
            var qmpVer = "";
            try {
                qmpVer = QzonePlayer.GetVersion(4)
            } catch(e) {
                throw new Error("NeedUpdateQzoneMusic");
                return false
            }
            if (! (qmpVer >= "7.69")) {
                throw new Error("NeedUpdateQzoneMusic");
                return false
            }
            QzonePlayer.CreateAX("QzoneMusic.dll");
            for (var k in args) switch (k) {
            case "classid":
            case "style":
            case "name":
            case "height":
            case "width":
            case "id":
            case "UsedWhirl":
                continue;
                break;
            default:
                QzonePlayer.setAttribute(k, args[k])
            }
            try {
                QzonePlayer.UsedWhirl = "0"
            } catch(e) {}
            if (QzonePlayer.GetVersion(5) >= "3.2") {
                QzonePlayer.setAttribute("P2PUDPServ_IP", "pdlmusic.p2p.qq.com");
                QzonePlayer.setAttribute("P2PTCPServ_IP", "pdlmusic.p2p.qq.com")
            }
            return QzonePlayer
        }
    }
    function createPlayer() {
        var ttii = parseInt(Math.random() * 1E3) % mQQPlayerConfig.REP_PLAYSONG_IP_ARRAY.length;
        var ttii2 = parseInt(Math.random() * 1E3) % mQQPlayerConfig.REP_SONGLIST_IP_ARRAY.length;
        var ttii3 = parseInt(Math.random() * 1E3) % mQQPlayerConfig.REP_PLAYURL_IP_ARRAY.length;
        var ttii4 = (new Date).getTime() % 2;
        if (plv >= "3.2") ttii4 = 2;
        return insertQQPlayer({
            classid: "CLSID:E05BC2A3-9A46-4A32-80C9-023A473F5B23",
            id: "QzonePlayer",
            height: 0,
            width: 0,
            PlayerType: 2,
            CacheSize: mQQPlayerConfig.P2P_CACHE_SPACE,
            ValidDomain: "qq.com",
            EnableSyncListen: 1,
            UploadStatCount: 10,
            ExitDelayTime: 5,
            UsedWhirl: 0,
            RestrictHttpStartInterval: 1,
            RestrictHttpStopInterval: 100,
            P2PUDPServ_IP: mQQPlayerConfig.P2P_UDP_SVR_IP_ARRAY[ttii4],
            P2PUDPServ_Port: mQQPlayerConfig.P2P_UDP_SVR_PORT,
            P2PTCPServ_IP: mQQPlayerConfig.P2P_TCP_SVR_IP_ARRAY[ttii4],
            P2PTCPServ_Port: mQQPlayerConfig.P2P_TCP_SVR_PORT,
            P2PStunServ_IP: mQQPlayerConfig.P2P_STUN_SVR_IP,
            P2PStunServ_Port: mQQPlayerConfig.P2P_STUN_SVR_PORT,
            RepPlaySong_IP: mQQPlayerConfig.REP_PLAYSONG_IP_ARRAY[ttii],
            RepPlaySong_Port: mQQPlayerConfig.REP_PLAYSONG_PORT,
            RepSongList_IP: mQQPlayerConfig.REP_SONGLIST_IP_ARRAY[ttii2],
            RepSongList_Port: mQQPlayerConfig.REP_SONGLIST_PORT,
            RepPlayURL_IP: mQQPlayerConfig.REP_PLAYURL_IP_ARRAY[ttii3],
            RepPlayURL_Port: mQQPlayerConfig.REP_PLAYURL_PORT
        })
    }
    function createActiveX() {
        try {
            mPlayerName = createPlayer();
            if (!mPlayerName) return false;
            mPlayerSource = "web_player_" + (new Date).getTime();
            mCurPlaySrc = mPlayerSource
        } catch(e) {
            if (debugMode) alert("e 7 " + e.message);
            throw new Error("NeedUpdateQzoneMusic");
            return false
        }
        return true
    }
    function initialize() {
        try {
            if (!mPlayerName) return false;
            EventPlayer(mPlayerName, "OnInitialized", g_playerCallback.OnInitialized);
            EventPlayer(mPlayerName, "OnUninitialized", g_playerCallback.OnUnitialized);
            EventPlayer(mPlayerName, "OnStateChanged", g_playerCallback.OnStateChanged);
            EventPlayer(mPlayerName, "OnPlayProgress", g_playerCallback.OnPlayProgress);
            EventPlayer(mPlayerName, "OnPlayError", g_playerCallback.OnPlayError);
            EventPlayer(mPlayerName, "OnDnldProgress", g_playerCallback.OnDownloadProgress);
            EventPlayer(mPlayerName, "OnPlaySrcChanged", g_playerCallback.OnPlaySrcChanged);
            mPlayerName.Initialize();
            mPlayerName.Volume = 75
        } catch(e) {
            if (debugMode) alert("e 8 " + e.message);
            return false
        }
        mIsInit = true;
        return true
    }
    function unInitialize() {
        try {
            EventPlayerRemove(mPlayerName, "OnInitialized", g_playerCallback.OnInitialized);
            EventPlayerRemove(mPlayerName, "OnUninitialized", g_playerCallback.OnUnitialized);
            EventPlayerRemove(mPlayerName, "OnStateChanged", g_playerCallback.OnStateChanged);
            EventPlayerRemove(mPlayerName, "OnPlayProgress", g_playerCallback.OnPlayProgress);
            EventPlayerRemove(mPlayerName, "OnPlayError", g_playerCallback.OnPlayError);
            EventPlayerRemove(mPlayerName, "OnDnldProgress", g_playerCallback.OnDownloadProgress);
            EventPlayerRemove(mPlayerName, "OnPlaySrcChanged", g_playerCallback.OnPlaySrcChanged);
            if (mPlayerName.Uninitialize()) {
                mIsInit = false;
                g_playerCallback.OnUnitialized();
                return true
            }
            g_playerCallback.OnUnitialized()
        } catch(e) {
            if (debugMode) alert("e 9 " + e.message);
            g_playerCallback.OnUnitialized();
            return false
        }
    }
    function setPlayerSrc() {
        if (!playerSrcSeted) {
            mPlayerName.PlaySrc = mPlayerSource;
            playerSrcSeted = true
        }
    }
    function setPlayURL() {
        if (getCookie("qqmusic_uin") == "" || (getCookie("qqmusic_key") == "" || getCookie("qqmusic_fromtag") == "")) {
            setCookie("qqmusic_uin", mUinCookie, "qq.com");
            setCookie("qqmusic_key", mKeyCookie, "qq.com");
            setCookie("qqmusic_fromtag", mFromTag, "qq.com")
        }
        var _obj = g_webPlayer.getSongInfoObj();
        if (!mIsInit) return;
        g_webPlayer.getSongUrl(_obj,
        function(playUrl) {
            if (playUrl == "") {
                alert("\u6b4c\u66f2\u94fe\u63a5\u9519\u8bef\uff01");
                return
            }
            setPlayerSrc();
            var sid = 0;
            if ( !! _obj.mid);
            var torrentUrl = g_webPlayer.getTptUrl(_obj);
            setPlayParams(sid, playUrl);
            mPlayerName.SetPlayURL(sid, playUrl, torrentUrl)
        });
        return
    }
    function isPlaying() {
        if (!mIsInit) return false;
        return mPlayerState == g_playerStatus.S_PLAYING || (mPlayerState == g_playerStatus.S_BUFFERING || mPlayerState == g_playerStatus.S_PLAYBEGIN)
    }
    function isPause() {
        if (!mIsInit) return false;
        return mPlayerState == g_playerStatus.S_PAUSE
    }
    function isStop() {
        if (!mIsInit) return false;
        var _s = mPlayerState;
        if (_s == g_playerStatus.S_BUFFERING) {
            var cur = (new Date).getTime();
            if (cur - lastBufTime > 1E3 * 60) lastBufTime = (new Date).getTime();
            if (cur - lastBufTime > 1E3 * 20) {
                lastBufTime = (new Date).getTime();
                return true
            }
        } else lastBufTime = 0;
        return _s == g_playerStatus.S_STOP || _s == g_playerStatus.S_PLAYEND
    }
    function startPlayer() {
        if (!mIsInit) return false;
        if (isPlaying()) return false;
        try {
            mPlayerName.Play();
            return true
        } catch(e) {
            if (debugMode) alert("e 11 " + e.message)
        }
        return false
    }
    function stopPlayer() {
        if (!mIsInit) return false;
        try {
            mPlayerName.Stop();
            return true
        } catch(e) {
            if (debugMode) alert("e 12 " + e.message)
        }
        return false
    }
    function pausePlayer() {
        if (!mIsInit) return false;
        if (!isPlaying()) return false;
        try {
            mPlayerName.Pause()
        } catch(e) {
            if (debugMode) alert("e 13 " + e.message)
        }
    }
    function setMute() {
        if (!mIsInit) return false;
        var bSet;
        var isIE = window.ActiveXObject ? true: false;
        if (isIE) bSet = mPlayerName.Mute == 0 ? 1 : 0;
        else bSet = mPlayerName.Mute ? false: true;
        mPlayerName.Mute = bSet;
        return bSet
    }
    function getVolumn() {
        if (!mIsInit) return 0;
        return mPlayerName.Volume
    }
    function setVolumn(vol) {
        if (!mIsInit) return false;
        if (mPlayerName.Mute == 1) return false;
        if (vol > 100) vol = 100;
        if (vol < 0) vol = 0;
        if (vol >= 0 && vol <= 100) mPlayerName.Volume = vol;
        return true
    }
    function setPlayerState(status) {
        mPlayerState = status
    }
    function setPlayProgress(curtime) {
        if (!mIsInit) return false;
        if (isNaN(curtime)) return false;
        if (curtime <= 0) curtime = 0;
        if (curtime >= g_webPlayer.getSongDuration()) curtime = g_webPlayer.getSongDuration();
        mPlayerName.CurPos = curtime;
        return true
    }
    function setDownloadProgress(progress) {
        if (!mIsInit) return false;
        if (progress <= 0) progress = 0;
        if (progress >= 100) progress = 100;
        mDownloadProgress = progress
    }
    function getDownloadProgress() {
        if (!mIsInit) return false;
        return mDownloadProgress || 0
    }
    return {
        createActiveX: createActiveX,
        setPlayerVersion: setPlayerVersion,
        initialize: initialize,
        unInitialize: unInitialize,
        setPlayURL: setPlayURL,
        setPlayList: setPlayList,
        resetCache: resetCache,
        startPlayer: startPlayer,
        stopPlayer: stopPlayer,
        pausePlayer: pausePlayer,
        setMute: setMute,
        getVolumn: getVolumn,
        setVolumn: setVolumn,
        setPlayerState: setPlayerState,
        getPlayerSource: getPlayerSource,
        getCurrentPlayerSource: getCurrentPlayerSource,
        setCurrentPlayerSource: setCurrentPlayerSource,
        setPlayProgress: setPlayProgress,
        setDownloadProgress: setDownloadProgress,
        getDownloadProgress: getDownloadProgress
    }
};
function EventPlayer(oTarget, sEventType, fnHandler) {
    if (oTarget.attachEvent) oTarget.attachEvent(sEventType, fnHandler);
    else if (oTarget.addEventListener) oTarget.addEventListener(sEventType, fnHandler, false);
    else oTarget[sEventType] = fnHandler
}
function EventPlayerRemove(oTarget, sEventType, fnHandler) {
    if (oTarget.detachEvent) oTarget.detachEvent(sEventType, fnHandler);
    else if (oTarget.removeEventListener) oTarget.removeEventListener(sEventType, fnHandler, false);
    else oTarget[sEventType] = null
}
function EventUtil(oTarget, sEventType, fnHandler) {
    if (oTarget.attachEvent) oTarget.attachEvent("on" + sEventType, fnHandler);
    else if (oTarget.addEventListener) oTarget.addEventListener(sEventType, fnHandler, false);
    else oTarget["on" + sEventType] = fnHandler
}
MUSIC.module.webPlayer.eventCallback = function() {
    var $T = top,
    $ = MUSIC,
    $D = $.dom,
    $E = $.event;
    function OnInitialized(bSucc) {
        if (bSucc);
        else alert("webPlayer initialize faied")
    }
    function OnUnitialized() {}
    function OnStateChanged(lNewState) {
        if (debugMode) alert("OnStateChanged:" + lNewState);
        if (lNewState >= 0 && lNewState <= 6) g_webPlayer.setPlayerState(lNewState);
        switch (lNewState) {
        case 0:
            g_webPlayer.setPlayerState(g_playerStatus.S_UNDEFINE);
            break;
        case 1:
            g_webPlayer.setPlayerState(g_playerStatus.S_STOP);
            g_webPlayer.OnPlayStop();
            break;
        case 2:
            g_webPlayer.setPlayerState(g_playerStatus.S_PAUSE);
            g_webPlayer.OnPlayPause();
            break;
        case 3:
            g_webPlayer.setPlayerState(g_playerStatus.S_PLAYING);
            g_webPlayer.OnPlaying();
            break;
        case 4:
            g_webPlayer.setPlayerState(g_playerStatus.S_BUFFERING);
            break;
        case 5:
            g_webPlayer.setPlayerState(g_playerStatus.S_PLAYBEGIN);
            MUSIC.OZ.speedSet(118, 4, MUSIC.expType, +new Date);
            MUSIC.OZ.speedSend(118, 4, 1);
            break;
        case 6:
            g_webPlayer.setPlayerState(g_playerStatus.S_PLAYEND);
            g_webPlayer.autoNextPlayer();
            break;
        default:
            break
        }
    }
    function OnPlayProgress(lCurPos, lTotal) {
        var _h5 = !!ua.safari || ( !! ua.chrome || ( !! ua.isiPad || !!ua.isiPhone));
        lCurPos = _h5 ? parseInt(lCurPos * 1E3) : parseInt(lCurPos);
        lTotal = parseInt(lTotal);
        g_webPlayer.setCurPostion(lCurPos, lTotal);
        g_webPlayer.OnSongPlaying(lCurPos, lTotal);
        if (debugMode) alert("PlayProgress,curPos:" + lCurPos + ",total:" + lTotal)
    }
    function OnPlayError(lErrCode, sErrDesc) {
        cs.p("g_playerCallback.OnPlayError 1");
        var _obj = g_webPlayer.getSongInfoObj();
        var index = g_webPlayer.getPostion();
        cs.p("g_playerCallback.OnPlayError 2,_obj:", _obj, "index:", index);
        g_webPlayer.OnPlayError(_obj, index);
        cs.p("g_playerCallback.OnPlayError 3");
        if (debugMode) alert("playError,ErrCode:" + lErrCode + ",ErrDesc:" + sErrDesc)
    }
    function OnPlaySrcChanged(sNewPlaySrc, sOldPlaySrc) {
        g_webPlayer.setCurrentPlayerSource(sNewPlaySrc);
        if (g_webPlayer.getCurrentPlayerSource() != g_webPlayer.getPlayerSource()) {
            g_webPlayer.setPlayerState(g_playerStatus.S_PAUSE);
            g_webPlayer.OnPlayPause()
        }
    }
    function OnDownloadProgress(curPos, downloadProgress) {
        g_webPlayer.setDownloadProgress(downloadProgress);
        g_webPlayer.OnSongDownloading(downloadProgress)
    }
    return {
        OnInitialized: OnInitialized,
        OnUnitialized: OnUnitialized,
        OnStateChanged: OnStateChanged,
        OnPlayProgress: OnPlayProgress,
        OnPlayError: OnPlayError,
        OnPlaySrcChanged: OnPlaySrcChanged,
        OnDownloadProgress: OnDownloadProgress
    }
} ();
function statImgSend(url, t) {
    if (!window.tmpMusicStat) window.tmpMusicStat = [];
    var l = window.tmpMusicStat.length;
    window.tmpMusicStat[l] = new Image;
    with(window.tmpMusicStat[l]) onload = onerror = new Function("this.ready=true;this.onload=this.onerror=null;statImgClean();");
    window.setTimeout("window.tmpMusicStat[" + l + "].src = '" + url + "';", t)
}
function statImgClean() {
    for (var i = 0,
    l = window.tmpMusicStat.length; i < l; i++) if ( !! window.tmpMusicStat[i] && !!window.tmpMusicStat[i].ready) delete window.tmpMusicStat[i]
}
MUSIC.module.webPlayer.stat = function() {
    var _fromtag1 = 0,
    _num = 5,
    _guid = "",
    _statList = [];
    function _getGuid() {
        if (_guid.length > 0) return _guid;
        var u = getCookie("pgv_pvid");
        if ( !! u && u.length > 0) {
            _guid = u;
            return _guid
        }
        var curMs = (new Date).getUTCMilliseconds();
        _guid = Math.round(Math.random() * 2147483647) * curMs % 1E10;
        document.cookie = "pgv_pvid=" + _guid + "; Expires=Sun, 18 Jan 2038 00:00:00 GMT; PATH=/; DOMAIN=qq.com;";
        return _guid
    }
    function add(songobj) {
        var o = {
            id: 0,
            type: 0,
            playtime: 0,
            starttime: 0,
            fromtag2: 0
        };
        var len = _statList.length;
        if (len > 0) {
            var _h5 = !!ua.safari || ( !! ua.chrome || ( !! ua.isiPad || !!ua.isiPhone));
            var ptime = g_webPlayer.getCurPostion();
            if (_h5) ptime = ptime / 1E3;
            _statList[len - 1].playtime = parseInt(ptime)
        }
        if (typeof songobj == "object" && songobj != null) {
            if (len >= _num) submit();
            o.id = songobj.mid;
            o.fromtag2 = songobj.msource || g_webPlayer.getOption().statFromtag * 100;
            if ( !! songobj.mid && !!songobj.mstream) o.type = 3;
            else o.type = 1;
            o.starttime = parseInt((new Date).getTime() / 1E3, 10);
            _statList.push(o)
        } else submit(true)
    }
    function submit(noTimeout) {
        noTimeout = noTimeout || false;
        var o = null,
        id = [],
        type = [],
        playtime = [],
        starttime = [],
        fromtag2 = [];
        _playerType = g_webPlayer.getPlayerType();
        _fromtag1 = g_webPlayer.getOption().statFromtag;
        var count = _statList.length;
        for (var i = 0; i < count; i++) {
            o = _statList[i];
            id.push(parseInt(o.id) < 1 ? 0 : o.id);
            type.push(o.type || 0);
            playtime.push(o.playtime || 0);
            starttime.push(o.starttime || 0);
            fromtag2.push(o.fromtag2 || 0)
        }
        if (count > 0) {
            var statUrl = "http://pt.music.qq.com/fcgi-bin/cgi_music_webreport.fcg?Count=" + count + "&Fqq=" + g_user.getUin() + "&Fguid=" + _getGuid() + "&Ffromtag1=" + _fromtag1 + "&Ffromtag2=" + fromtag2.join(",") + "&Fsong_id=" + id.join(",") + "&Fplay_time=" + playtime.join(",") + "&Fstart_time=" + starttime.join(",") + "&Ftype=" + type.join(",") + "&Fversion=" + _playerType;
            if (noTimeout) {
                _img = new Image;
                _img.src = statUrl
            } else statImgSend(statUrl, 0)
        }
        id = null;
        type = null;
        playtime = null;
        starttime = null;
        fromtag2 = null;
        _statList = []
    }
    return {
        add: add
    }
} (); (function() {
    if ( !! ua) ua.tt = function() {
        var vtt = NaN;
        var agent = /(?:(?:TencentTraveler|QQBrowser).(\d+\.\d+))/.exec(navigator.userAgent);
        if (agent) vtt = agent[1] ? parseFloat(agent[1]) : NaN;
        else vtt = NaN;
        return vtt
    } ()
})();
var g_player = MUSIC.module.webPlayer;
var g_webPlayer = g_player.interFace;
var g_playerList = g_player.playerList;
var g_playerStatus = g_player.playStatus;
var g_playerCallback = g_player.eventCallback;
var g_playerStat = g_player.stat;
MUSIC.event.getWheelDelta = function(event) {
    if (event.wheelDelta) return event.wheelDelta;
    else return - event.detail * 40
};
MUSIC.scrollbar = function() {
    var E = MUSIC.event;
    var D = MUSIC.dom;
    var bar, barp, cont, contp, barsz, barpsz, contsz, contpsz, rate, i = 0,
    bmax, isScroll = false,
    intf, delta, bnewy, cnewy, barclassname, cmax, bmax, _options = {
        bartop: "",
        barleft: 0,
        bar_hover: "",
        wheel_len: 50,
        wheel_num: 2,
        interval: 2
    };
    function init(options) {
        bar = D.get(options.barid);
        cont = D.get(options.contid);
        barp = bar.parentNode;
        barp.style.display = "";
        contp = cont.parentNode;
        contsz = D.getSize(D.get(options.ulid));
        barpsz = D.getSize(barp);
        contpsz = D.getSize(contp);
        barclassname = bar.className;
        ullen = D.getSize(D.get(options.ulid))[1];
        var rate1 = ullen / barpsz[1] || 1;
        var newheight = parseInt(barpsz[1] / rate1);
        _options.barleft = D.getStyle(bar, "left");
        if (_options.bartop === "") _options.bartop = 0;
        else {
            D.setXY(cont, 0, 0);
            D.setXY(bar, _options.barleft, _options.top)
        }
        _options.bar_hover = barclassname;
        MUSIC.object.extend(_options, options || {});
        if (newheight < 20) newheight = 20;
        if (newheight > barpsz[1] - _options.bartop * 2) {
            bar.style.display = "none";
            barp.style.display = "none";
            if (isScroll) {
                clearInterval(intf);
                isScroll = false
            }
            E.addEvent(bar, "mousedown", setBar);
            E.addEvent(document, "mouseup",
            function() {
                E.removeEvent(document, "mousemove", showchange, arr);
                bar.className = barclassname
            });
            E.addEvent(MUSIC.dom.get("contentFrame"), "mouseout",
            function() {
                E.removeEvent(document, "mousemove", showchange);
                bar.className = barclassname
            });
            barsz = D.getSize(bar);
            var arr = {
                "top": 0,
                "ey": 0
            };
            E.addEvent(barp, "click", setBarp, arr); (function() {
                E.addEvent(contp, "mousewheel", setWheel);
                E.addEvent(contp, "DOMMouseScroll", setWheel)
            })();
            bmax = 0;
            cmax = 0;
            return
        }
        bar.style.height = newheight + "px";
        bar.style.display = "none";
        bar.style.display = "";
        barsz = D.getSize(bar);
        if (isScroll) {
            clearInterval(intf);
            isScroll = false
        }
        rate = (ullen - contpsz[1]) / (barpsz[1] - barsz[1] - _options.bartop);
        bmax = barpsz[1] - barsz[1] - 2 * _options.bartop - 1;
        cmax = contsz[1] - contpsz[1];
        E.addEvent(bar, "mousedown", setBar);
        E.addEvent(document, "mouseup",
        function() {
            E.removeEvent(document, "mousemove", showchange, arr);
            bar.className = barclassname
        });
        E.addEvent(MUSIC.dom.get("contentFrame"), "mouseout",
        function() {
            E.removeEvent(document, "mousemove", showchange);
            bar.className = barclassname
        });
        var arr = {
            "top": 0,
            "ey": D.getXY(bar)[1] + barsz[1] / 2
        };
        E.addEvent(barp, "click", setBarp, arr); (function() {
            E.addEvent(contp, "mousewheel", setWheel);
            E.addEvent(contp, "DOMMouseScroll", setWheel)
        })()
    }
    function setBarp(e, arr) {
        var target = E.getTarget(e);
        if (target == this) showchange(e, arr)
    }
    function showchange(e, arr) {
        var event = E.getEvent(e);
        E.preventDefault(e);
        var y = E.mouseY(event);
        var moveY = y - arr.ey;
        var newy = moveY + arr.top;
        var cnewy = -newy * rate;
        if (newy <= _options.bartop) {
            newy = _options.bartop;
            cnewy = 0
        } else if (newy > bmax) {
            newy = bmax;
            cnewy = -cmax
        }
        if (cnewy + cmax < 0) cnewy = -cmax;
        D.setXY(cont, 0, cnewy);
        D.setXY(bar, _options.barleft, newy)
    }
    function setBar(e) {
        var event = E.getEvent(e);
        E.preventDefault(event);
        var barxy = D.getXY(bar);
        var ey = E.mouseY(event);
        var top = barxy[1] - D.getXY(barp)[1];
        var arr = {
            "top": top,
            "ey": ey
        };
        E.addEvent(document, "mousemove", showchange, arr);
        bar.className = _options.bar_hover
    }
    function changewheel() {
        if (delta + 1) i++;
        else i--;
        if (i == 0) {
            clearInterval(intf);
            isScroll = false
        }
        bnewy += -delta * _options.wheel_len / rate;
        cnewy += delta * _options.wheel_len;
        if (bnewy < _options.bartop) {
            bnewy = _options.bartop;
            i == 0
        } else if (bnewy > bmax) {
            bnewy = bmax;
            i == 0
        }
        if (cnewy > 0) cnewy = 0;
        else if (cmax + cnewy < 0) cnewy = -cmax;
        D.setXY(cont, 0, cnewy);
        D.setXY(bar, _options.barleft, bnewy)
    }
    function setWheel(e) {
        var event = E.getEvent(e);
        E.preventDefault(event);
        E.cancelBubble();
        delta = E.getWheelDelta(event) / 120;
        if (!isScroll) {
            isScroll = true;
            bnewy = D.getXY(bar)[1] - D.getXY(barp)[1];
            cnewy = D.getXY(cont)[1] - D.getXY(contp)[1];
            if (delta + 1) i = -_options.wheel_num;
            else i = _options.wheel_num;
            intf = setInterval(changewheel, _options.interval)
        } else if (delta + 1) if (i < 0) i = i - _options.wheel_num;
        else i = -_options.wheel_num;
        else if (i < 0) i = _options.wheel_num;
        else i = i + _options.wheel_num
    }
    function selectElement(element) {
        if (bar.style.display == "none") return;
        else {
            var element = D.get(element);
            var position = D.getXY(element);
            var contParentXY = D.getXY(contp);
            var cnewy = position[1] - contParentXY[1] + D.getPosition(element).height - D.getPosition(cont).top + MUSIC.dom.getScrollTop();
            if (position[1] + D.getPosition(element).height > contParentXY[1] && position[1] < contParentXY[1] + contpsz[1]) return;
            var bnewy = cnewy / rate;
            if (bnewy <= _options.bartop + 10) {
                bnewy = _options.bartop;
                cnewy = 0
            } else if (bnewy > bmax) {
                bnewy = bmax;
                cnewy = cmax
            }
            if (cnewy > cmax) cnewy = cmax;
            else if (cnewy < 0) cnewy = 0;
            D.setXY(cont, 0, -cnewy);
            D.setXY(bar, _options.barleft, bnewy)
        }
    }
    function refresh() {
        var currentbar = D.getXY(bar)[1] - D.getXY(barp)[1],
        currentcont = -D.getXY(cont)[1];
        bar.style.display = "";
        barp.style.display = "";
        contsz = D.getSize(D.get(_options.ulid));
        barpsz = D.getSize(barp);
        contpsz = D.getSize(contp);
        ullen = D.getSize(D.get(_options.ulid))[1];
        var rate1 = ullen / barpsz[1];
        if (rate1 == 0) rate1 = 1;
        var newheight = parseInt(barpsz[1] / rate1);
        _options.barleft = D.getStyle(bar, "left");
        if (newheight < 20) newheight = 20;
        if (newheight >= barpsz[1] - _options.bartop * 2) {
            D.setXY(cont, 0, 0);
            D.setXY(bar, _options.barleft, _options.top);
            bar.style.display = "none";
            barp.style.display = "none";
            if (isScroll) {
                clearInterval(intf);
                isScroll = false
            }
            cmax = 0;
            bmax = 0;
            return
        }
        bar.style.height = newheight + "px";
        bar.style.display = "none";
        bar.style.display = "";
        barsz = D.getSize(bar);
        if (isScroll) {
            clearInterval(intf);
            isScroll = false
        }
        rate = (ullen - contpsz[1]) / (barpsz[1] - barsz[1] - _options.bartop);
        bmax = barpsz[1] - barsz[1] - 2 * _options.bartop - 1;
        cmax = contsz[1] - contpsz[1];
        if (currentbar <= _options.bartop + 10) {
            currentbar = _options.bartop;
            currentcont = 0;
            D.setXY(cont, 0, -currentcont);
            D.setXY(bar, _options.barleft, currentbar)
        } else if (currentbar > bmax) {
            currentbar = bmax;
            currentcont = cmax;
            D.setXY(cont, 0, -currentcont);
            D.setXY(bar, _options.barleft, currentbar)
        }
        if (currentcont > cmax) {
            currentcont = cmax;
            D.setXY(cont, 0, -currentcont);
            D.setXY(bar, _options.barleft, currentbar)
        }
    }
    return {
        init: init,
        selectElement: selectElement,
        refresh: refresh
    }
};
MUSIC.channel.top_player = function() {
    var _curTab = "playlist",
    _curPlayDirid = -1,
    _curPlayElem = null,
    _isFirstShow = true,
    _curSettings = {
        volume: 75,
        mode: 3
    },
    _playListEmpty = true;
    _playlistBar = {},
    _favlistBar = {},
    $ = MUSIC,
    $C = $.css,
    $D = $.dom,
    $E = $.event;
    var _isPlaying = false;
    var _timerClose = null;
    function _startTime() {
        if (!_timerClose) _timerClose = setTimeout(function() {
            trogglePlayer("hide");
            _timerClose = null
        },
        1E4)
    }
    function _stopTime() {
        if (_timerClose) {
            clearTimeout(_timerClose);
            _timerClose = null
        }
    }
    function getACSRFToken() {
        function _DJB(str) {
            var hash = 5381;
            for (var i = 0,
            len = str.length; i < len; ++i) hash += (hash << 5) + str.charCodeAt(i);
            return hash & 2147483647
        }
        return _DJB(getCookie("skey") == "" ? getCookie("qqmusic_key") : getCookie("skey"))
    }
    var Events = function() {
        var slice = Array.prototype.slice,
        splice = Array.prototype.splice,
        eventSplitter = /\s+/,
        Events = {
            on: function(events, callback, context) {
                var calls, event, node, tail, list;
                if (!callback) return this;
                events = events.split(eventSplitter);
                calls = this._callbacks || (this._callbacks = {});
                while (event = events.shift()) {
                    list = calls[event];
                    node = list ? list.tail: {};
                    node.next = tail = {};
                    node.context = context;
                    node.callback = callback;
                    calls[event] = {
                        tail: tail,
                        next: list ? list.next: node
                    }
                }
                return this
            },
            off: function(events, callback, context) {
                var event, calls, node, tail, cb, ctx;
                if (! (calls = this._callbacks)) return;
                if (! (events || (callback || context))) {
                    delete this._callbacks;
                    return this
                }
                events = events ? events.split(eventSplitter) : _.keys(calls);
                while (event = events.shift()) {
                    node = calls[event];
                    delete calls[event];
                    if (!node || !(callback || context)) continue;
                    tail = node.tail;
                    while ((node = node.next) !== tail) {
                        cb = node.callback;
                        ctx = node.context;
                        if (callback && cb !== callback || context && ctx !== context) this.on(event, cb, ctx)
                    }
                }
                return this
            },
            trigger: function(events) {
                var event, node, calls, tail, args, all, rest;
                if (! (calls = this._callbacks)) return this;
                all = calls.all;
                events = events.split(eventSplitter);
                rest = slice.call(arguments, 1);
                while (event = events.shift()) {
                    if (node = calls[event]) {
                        tail = node.tail;
                        while ((node = node.next) !== tail) {
                            if (MUSIC.object.getType(node.callback) == "string") node.callback = eval(node.callback);
                            node.callback.apply(node.context || this, rest)
                        }
                    }
                    if (node = all) {
                        tail = node.tail;
                        args = [event].concat(rest);
                        while ((node = node.next) !== tail) {
                            if ($.type(node.callback) == "string") node.callback = eval(node.callback);
                            node.callback.apply(node.context || this, args)
                        }
                    }
                }
                return this
            }
        };
        return Events
    } ();
    var common = {
        isDirOk: function(dirinfo) {
            var uin = g_user.getUin();
            if (dirinfo.dirid == 205 || uin != dirinfo.owner.uin) return false;
            else return true
        },
        formatTime: function(seconds) {
            if (seconds <= 0) return "--:--";
            var mins = parseInt(seconds / 60, 10),
            secs = parseInt(seconds % 60, 10);
            return (mins > 9 ? "": "0") + mins + ":" + (secs > 9 ? "": "0") + secs
        },
        isQusicSong: function(songobj) {
            return MUSIC.lang.isHashMap(songobj) && ( !! songobj.mid && ( !! songobj.mstream && (songobj.mid > 0 && songobj.mstream > 0)))
        },
        showEle: function(ele) {
            ele && (ele.style && (ele.style.display = "block"))
        },
        hideEle: function(ele) {
            ele && (ele.style && (ele.style.display = "none"))
        }
    };
    var _localSong = {
        _list: null,
        _stoleLinkId: 0,
        get: function(callback) {
            if ( !! this._list) {
                callback(this._list);
                return
            }
            var _this = this;
            try {
                g_storage.get("qqmusic_playlist",
                function(data) {
                    top["qqmusic_playlist"] = [];
                    if ( !! data) if (window.JSON && window.JSON.parse) top["qqmusic_playlist"] = JSON.parse(data);
                    else eval('top["qqmusic_playlist"] = ' + data);
                    _this._list = [];
                    MUSIC.object.each(top["qqmusic_playlist"],
                    function(music) {
                        var _cache = g_trackServ.formatMusic(music);
                        if (!_cache.playid) _cache.playid = "j" + _this._stoleLinkId++;
                        _this._list.push(_cache)
                    });
                    callback(_this._list)
                })
            } catch(e) {
                callback([])
            }
        },
        save: function() {
            var list = [],
            count = 0,
            tmplist = [];
            for (var i = 0,
            len = this._list.length; i < len; i++) {
                var tmp = g_trackServ.buildMusicData(this._list[i]);
                count += tmp.length + 3;
                if ((ua.ie == 6 || ua.ie == 7) && count >= 32E3 || i >= 300) break;
                tmplist.push(this._list[i]);
                list.push(tmp)
            }
            g_storage.set("qqmusic_playlist", MUSIC.lang.obj2str(list));
            this._list = tmplist;
            return list.length
        },
        add: function(newlist) {
            for (var i = 0,
            len = newlist.length; i < len; i++) {
                newlist[i].playid = "j" + this._stoleLinkId++;
                this._list.push(newlist[i])
            }
            return this.save()
        },
        del: function(index) {
            if (index >= 0 && index < this._list.length) {
                this._list.splice(index, 1);
                this.save()
            }
        },
        clear: function() {
            this._list = [];
            this.save()
        },
        insert: function(index, songinfo) {
            if (index >= 0 && index < this._list.length) this._list.splice(index, 0, songinfo);
            return this.save()
        }
    };
    var listFrame = function() {
        var _list = $D.get("divsonglist"),
        _hoverEle = null,
        _setEvent = function(removeEvent) {
            var _fun = removeEvent ? $E.removeEvent: $E.addEvent,
            _listClick = function(e) {
                var _target = $E.getTarget(e),
                _className = _target.className,
                _liElement = function() {
                    var _cache = _target;
                    while (_cache && !/li/ig.test(_cache.tagName)) _cache = _cache.parentNode;
                    return _cache
                } ();
                if (/btn_like/ig.test(_className)) {
                    listFrame.getSelectSongInfo(_liElement,
                    function(songInfo) {
                        _playerOp.like(songInfo, _target)
                    });
                    return
                }
                if (/btn_share/ig.test(_className)) {
                    listFrame.getSelectSongInfo(_liElement,
                    function(songInfo) {
                        _playerOp.share(_liElement, songInfo)
                    });
                    return
                }
                if (/btn_fav/ig.test(_className)) {
                    listFrame.getSelectSongInfo(_liElement,
                    function(songInfo) {
                        _playerOp.fav(_liElement)
                    });
                    return
                }
                if (/btn_del/ig.test(_className)) {
                    _playerOp.del(_liElement);
                    return
                }
                _playerOp.play(_liElement)
            },
            _listHover = function(e) {
                var _target = $E.getTarget(e),
                _name = _target.tagName;
                while (_target && !/li/ig.test(_target.tagName)) _target = _target.parentNode;
                if (!_target || !/li/ig.test(_target.tagName)) return;
                if (_hoverEle && !$C.hasClassName(_hoverEle, "class_fav")) $C.removeClassName(_hoverEle, "play_hover");
                _hoverEle = _target;
                $C.addClassName(_target, "play_hover")
            },
            _listOut = function(e) {
                if (_hoverEle && !$C.hasClassName(_hoverEle, "class_fav")) $C.removeClassName(_hoverEle, "play_hover");
                _hoverEle = null
            };
            _fun(_list, "click", _listClick);
            _fun(_list, "mouseover", _listHover);
            _fun(_list, "mouseout", _listOut)
        };
        return {
            init: function(list) {
                this.renderSongList(list);
                _setEvent()
            },
            setPlayList: function(dirid, callback) {
                if (dirid == _curPlayDirid) callback();
                else if (dirid == 0) _localSong.get(function(list) {
                    g_webPlayer.setPlayerList(false, list, _curSettings.mode);
                    _curPlayDirid = dirid;
                    callback()
                });
                else {
                    var uin = g_user.getUin();
                    g_mymusicAlbumMod.getDetail(uin, dirid,
                    function(data) {
                        g_webPlayer.setPlayerList(false, data.list, _curSettings.mode);
                        _curPlayDirid = dirid;
                        callback()
                    })
                }
            },
            getSelectSongInfo: function(obj, callback) {
                var pos = this.getSelectPos(obj),
                dirid = obj.parentNode.parentNode.getAttribute("dirid"),
                callback = callback ||
                function() {};
                if (pos >= 0 && dirid >= 0) if (dirid == 0) _localSong.get(function(list) {
                    if (pos < list.length) {
                        var music = list[pos];
                        music.dirid = dirid;
                        callback(music)
                    }
                });
                else {
                    var uin = g_user.getUin();
                    g_mymusicAlbumMod.getDetail(uin, dirid,
                    function(data) {
                        if (pos < data.list.length) {
                            var music = data.list[pos];
                            music.dirid = dirid;
                            callback(music)
                        }
                    })
                }
            },
            getSelectPos: function(obj) {
                if (!obj) return - 1;
                var _mid = obj.getAttribute("mid");
                for (var i = _localSong._list.length - 1; i >= 0; i--) if (_mid == _localSong._list[i].playid) return i;
                return - 1
            },
            renderSongList: function(list) {
                var uin = g_user.getUin();
                if (uin < 10001) _list.innerHTML = "<ul>" + listFrame.getSongListHtml(list) + "</ul>";
                else {
                    $D.get("divsonglist").innerHTML = "<ul>" + listFrame.getSongListHtml(list) + "</ul>";
                    g_mymusicAlbumMod.Song.getMyfavMap(uin,
                    function(mapMyfav) {
                        _list.innerHTML = "<ul>" + listFrame.getSongListHtml(list, mapMyfav) + "</ul>"
                    })
                }
                return this
            },
            addSongHtml: function(list) {
                var _es = $D.get("divsonglist").getElementsByTagName("ul");
                if (_es.length > 0) {
                    var uin = g_user.getUin();
                    if (uin < 10001) $D.insertAdjacent(_es[0], 2, listFrame.getSongListHtml(list));
                    else g_mymusicAlbumMod.Song.getMyfavMap(uin,
                    function(mapMyfav) {
                        $D.insertAdjacent(_es[0], 2, listFrame.getSongListHtml(list, mapMyfav))
                    })
                }
            },
            getSongListHtml: function(list, mapMyfav) {
                var _tpl_song = ['<li mid="%(playid)">', '<strong class="music_name" title="%(msong)">%(msong)</strong>', '<strong class="singer_name" title="%(msinger)">%(msinger)</strong>', '<strong class="play_time">%(time)</strong>', '<div class="list_cp">', '<span class="data">%(songdata)</span>', '<strong class="%(class_like)" title="%(title_like)" name="myfav_%(mmid)" mid="%(mmid)"><span>\u6211\u559c\u6b22</span></strong>', '<strong class="%(class_share)" title="%(title_share)"><span>\u5206\u4eab</span></strong>', '<strong class="%(class_fav)" title="%(title_fav)"><span>\u6536\u85cf</span></strong>', '<strong class="btn_del" title="\u4ece\u5217\u8868\u4e2d\u5220\u9664"><span>\u5220\u9664</span></strong>', "</div>", "</li>"].join("");
                var html = [];
                MUSIC.object.each(list,
                function(_music) {
                    var music = {};
                    MUSIC.object.extend(music, _music);
                    var _gososo = parseInt(music.gososo),
                    _down = parseInt(music.mdownload);
                    if (_gososo != 0 || _down != 1) {
                        music.class_like = "btn_like_n";
                        music.title_like = "\u6682\u4e0d\u63d0\u4f9b\u6b64\u6b4c\u66f2\u670d\u52a1";
                        music.func_like = "";
                        music.class_share = "btn_share_n";
                        music.title_share = "\u6682\u4e0d\u63d0\u4f9b\u6b64\u6b4c\u66f2\u670d\u52a1";
                        music.func_share = "";
                        music.class_fav = "btn_fav_n";
                        music.title_fav = "\u6682\u4e0d\u63d0\u4f9b\u6b64\u6b4c\u66f2\u670d\u52a1";
                        music.func_fav = ""
                    } else {
                        if ( !! mapMyfav && mapMyfav[music.mmid]) {
                            music.class_like = "btn_liked";
                            music.title_like = "\u53d6\u6d88\u559c\u6b22"
                        } else {
                            music.class_like = "btn_like";
                            music.title_like = "\u559c\u6b22"
                        }
                        music.class_share = "btn_share";
                        music.title_share = "\u5206\u4eab";
                        music.class_fav = "btn_fav";
                        music.title_fav = "\u6536\u85cf\u5230\u6b4c\u5355"
                    }
                    music.time = common.formatTime(music.minterval);
                    html.push(_tpl_song.jstpl_format(music))
                });
                return html.join("")
            }
        }
    } ();
    function _updateMode(mode) {
        var _class_list = ["cycle_single_bt", "ordered_bt", "cycle_bt", "unordered_bt"],
        _text_list = ["\u5355\u66f2\u5faa\u73af", "\u987a\u5e8f\u64ad\u653e", "\u5217\u8868\u5faa\u73af", "\u968f\u673a\u64ad\u653e"],
        _e = $D.get("btnPlayway");
        if (mode > 0 && mode <= 4) {
            _e.innerHTML = "<span>" + _text_list[mode - 1] + "</span>";
            _e.title = _text_list[mode - 1];
            _e.className = _class_list[mode - 1]
        }
        _curSettings.mode = mode;
        g_webPlayer.setMode(mode)
    }
    function _updateListScroll() {
        var _e = MUSIC.dom.get("divplayframe");
        if (_e.style.display == "none") return;
        _playlistBar.refresh();
        if (_curPlayElem) _playlistBar.selectElement(_curPlayElem)
    }
    function showTips(title, desc, num, style) {
        num = num || 1;
        var _tpl = ['<a title="\u5173\u95ed\u63d0\u793a" class="close_tips" href="javascript:;" onclick="g_topPlayer.hideTips();">\u5173\u95ed\u63d0\u793a</a>', "<div>", "<h6>%(title)</h6>", "<p>%(desc)</p>", "</div>", '<i class="%(class_arrow)"></i>'].join("");
        var _e = $D.get("btnfold"),
        _elem = $D.get("divyellowtips");
        if (!_elem) _elem = $D.createElementIn("div", null, false, {
            "class": "mod_tips",
            "id": "divyellowtips",
            "style": style || "width:230px;"
        });
        var data = {
            title: title,
            desc: desc
        };
        if (_e.title == "\u70b9\u51fb\u5c55\u5f00") data.class_arrow = "icon_arrow_left";
        else data.class_arrow = "icon_arrow_foot";
        _elem.innerHTML = _tpl.jstpl_format(data);
        var _pos = MUSIC.dom.getRect(_e);
        showElement(_elem);
        if (_e.title == "\u70b9\u51fb\u5c55\u5f00") {
            _elem.style.left = _pos["left"] + 27 + "px";
            _elem.style.top = _pos["top"] + 10 + "px"
        } else {
            _elem.style.left = _pos["left"] - 87 + "px";
            _elem.style.top = _pos["top"] - (num > 1 ? 81 : 64) + "px"
        }
    }
    function hideTips() {
        hideElement("divyellowtips")
    }
    function showLrcTips() {
        var _tpl = ['<a title="\u5173\u95ed\u63d0\u793a" class="close_tips" href="javascript:;" onclick="g_topPlayer.hideLrcTips();">\u5173\u95ed\u63d0\u793a</a>', "<div>", "<p>%(desc)</p>", "</div>", '<i class="icon_arrow_foot"></i>'].join("");
        var _e = $D.get("btnfold"),
        _elem = $D.get("divlrctips");
        if (!_elem) _elem = $D.createElementIn("div", null, false, {
            "class": "mod_tips lyrics_tips",
            "id": "divlrctips",
            "style": "width:230px;left:500px;bottom:120px;z-index:1001;"
        });
        var data = {
            desc: "\u70b9\u51fb\u8fd9\u91cc<br />\u8fd8\u53ef\u4ee5\u5f00\u5173\u6b4c\u8bcd\u9762\u677f\u54e6"
        };
        _elem.innerHTML = _tpl.jstpl_format(data);
        var _pos = $D.getPosition(_e);
        showElement(_elem);
        if (_e.title == "\u70b9\u51fb\u5c55\u5f00");
        else;
    }
    function hideLrcTips() {
        hideElement("divlrctips");
        g_storage.set("qqmusic_hidelrctips", "true")
    }
    var _playerOp = function() {
        var _btnPlayStatus = "g_first_play",
        _setPlayBtnStatus = function(cname, status) {
            var _btnPlay = $D.get("btnplay"),
            _cname = cname || "pause",
            _status = status || "g_pause";
            if (_cname == "pause") {
                _btnPlay.className = "play_bt";
                _btnPlay.title = "\u64ad\u653e(P)"
            } else {
                _btnPlay.className = "pause_bt";
                _btnPlay.title = "\u6682\u505c(P)"
            }
            _btnPlayStatus = _status
        },
        _setQQstatus = function(isexit, musicid) {
            var _img = new Image;
            musicid = musicid || "74";
            _img.src = "http://t.plcloud.music.qq.com/fcgi-bin/fcg_reportlsting_web.fcg?musicid=" + musicid + "&isexit=" + isexit + "&g_tk=" + getACSRFToken() + "&_r=" + (new Date).valueOf();
            _img.style.width = "0";
            _img.style.height = "0"
        },
        _bindEvent = function() {
            $D.get("btnplay").onclick = function() {
                var _status = _btnPlayStatus;
                if (_status == "g_first_play") {
                    $E.cancelBubble();
                    g_topPlayer.play();
                    return
                }
                if (_status == "g_play") {
                    $E.cancelBubble(); ! singleFm.leaving && singleFm.pause();
                    g_webPlayer.pausePlayer();
                    if (g_qrcHandlerChn.haveqrc) g_qrcHandlerChn.stopAnimFrame();
                    if (g_lrcHandlerChn.haveLrc) g_lrcHandlerChn.stopAnimFrame();
                    if ( !! g_msPinnedSite) {
                        g_msPinnedSite.paramsConfig.isPlaying = false;
                        g_msPinnedSite.paramsConfig.isPaused = true;
                        g_msPinnedSite.trogglePlayPauseBtn()
                    }
                    return
                }
                if (_status == "g_pause") {
                    $E.cancelBubble();
                    g_webPlayer.startPlayer();
                    if (g_qrcHandlerChn.haveqrc) g_qrcHandlerChn.startAnimFrame("qrc_ctn");
                    else if (g_lrcHandlerChn.haveLrc) g_lrcHandlerChn.startAnimFrame("qrc_ctn");
                    if ( !! g_msPinnedSite) {
                        g_msPinnedSite.paramsConfig.isPlaying = true;
                        g_msPinnedSite.paramsConfig.isPaused = false;
                        g_msPinnedSite.trogglePlayPauseBtn()
                    }
                }
                if (_status == "g_wait") g_webPlayer.startPlayer()
            }
        };
        return {
            _setQQstatus: _setQQstatus,
            _isPlaying: false,
            _isFirst: true,
            _curSongInfo: null,
            _errorPlayId: [],
            init: function() {
                g_webPlayer.OnSongPlayBegin = function(songinfo, index, total) {
                    try {
                        if (curtime > 0) g_musicMain.IP_limit.isLimit(function(isLimit) {
                            if ( !! isLimit) setTimeout(function() {
                                g_webPlayer.stopPlayer();
                                _playProgressBar.destroy();
                                _setPlayBtnStatus("pause", "g_first_play");
                                _playerOp.renderSong();
                                _playerOp._isPlaying = false;
                                _playerOp._curSongInfo = null;
                                $C.removeClassName($D.get("divplayer"), "m_player_playing")
                            },
                            0)
                        })
                    } catch(e) {}
                    _playerOp._isPlaying = true;
                    _playerOp._curSongInfo = songinfo;
                    var uin = g_user.getUin();
                    if (uin < 10001) _playerOp.renderSong(songinfo);
                    else g_mymusicAlbumMod.Song.getMyfavMap(uin,
                    function(mapMyfav) {
                        _playerOp.renderSong(songinfo, mapMyfav)
                    });
                    if (songinfo.mtype != "net") $D.get("qrc_ctn").innerHTML = "<p>&nbsp;</p><p>\u6b4c\u8bcd\u6b63\u5728\u52a0\u8f7d\u4e2d...</p>";
                    else $D.get("qrc_ctn").innerHTML = '<p>&nbsp;</p><p><a href="http://support.qq.com/write.shtml?fid=602" target="_blank">\u8be5\u5355\u66f2\u6682\u65e0\u6b4c\u8bcd\uff0c\u70b9\u51fb\u8fd9\u91cc\u8fdb\u884c\u53cd\u9988</a></p>';
                    g_qrcHandlerChn.clearQrcInfo();
                    common.isQusicSong(_playerOp._curSongInfo) && g_qrcHandlerChn.init({
                        songMId: songinfo.mmid,
                        songId: songinfo.mid,
                        qrcContainer: "qrc_ctn"
                    });
                    g_lrcHandlerChn.clearLrcInfo();
                    _volumeMod.init();
                    _playProgressBar.init();
                    var _ePlayer = $D.get("divplayer"),
                    _target = $D.get("btnfold");
                    if (_target.title == "\u70b9\u51fb\u5c55\u5f00") $C.addClassName(_ePlayer, "m_player_playing");
                    setTimeout(function() {
                        var _e = $D.get("divsonglist");
                        if ( !! _e) {
                            var _es = _e.getElementsByTagName("li");
                            if (_es.length > index) {
                                if (_curPlayElem) {
                                    $C.removeClassName(_curPlayElem, "play_current");
                                    _curPlayElem = null
                                }
                                _curPlayElem = _es[index];
                                $C.removeClassName(_curPlayElem, "play_wrong");
                                $C.addClassName(_curPlayElem, "play_current");
                                _updateListScroll()
                            }
                        }
                    },
                    500);
                    if ( !! g_msPinnedSite) {
                        g_msPinnedSite.paramsConfig.isPlaying = true;
                        g_msPinnedSite.paramsConfig.isPaused = false;
                        g_msPinnedSite.trogglePlayPauseBtn()
                    }
                    setTimeout(function() {
                        _setQQstatus(false, songinfo.mid)
                    },
                    700)
                };
                g_webPlayer.OnSongPlayEnd = function(songinfo, index, total) {
                    if (singleFm.status) {
                        var _time = songinfo.time || _playerOp._curSongInfo.minterval;
                        console.log(_time, _playerOp._curSongInfo.curSongTime);
                        if (_playerOp._curSongInfo.curSongTime == _time) singleFm.report(1, {
                            qq: g_user.getUin() || 0,
                            songid: songinfo.mid || _playerOp._curSongInfo.mid,
                            oper: 7,
                            rid: singleFm.rid,
                            sid: songinfo.mid || _playerOp._curSongInfo.mid,
                            time: _time,
                            playtime: _time
                        })
                    }
                    _playerOp._isPlaying = false;
                    var _ePlayer = $D.get("divplayer"),
                    _target = $D.get("btnfold");
                    if (_target.title == "\u70b9\u51fb\u5c55\u5f00") $C.removeClassName(_ePlayer, "m_player_playing");
                    _setPlayBtnStatus("pause", "g_play");
                    if (g_qrcHandlerChn.haveqrc) g_qrcHandlerChn.clearQrcInfo();
                    else if (g_lrcHandlerChn.haveLrc) g_lrcHandlerChn.clearLrcInfo();
                    if ( !! g_msPinnedSite) {
                        g_msPinnedSite.paramsConfig.isPlaying = false;
                        g_msPinnedSite.paramsConfig.isPaused = false;
                        g_msPinnedSite.trogglePlayPauseBtn()
                    }
                };
                g_webPlayer.OnSongPlaying = function(curtime, totaltime) {
                    if (isNaN(curtime) || isNaN(totaltime)) return;
                    try {
                        if (curtime > 0) g_musicMain.IP_limit.isLimit(function(isLimit) {
                            if (isLimit == 0) setTimeout(function() {
                                g_webPlayer.stopPlayer();
                                _playProgressBar.destroy();
                                _setPlayBtnStatus("pause", "g_first_play");
                                _playerOp.renderSong();
                                _playerOp._isPlaying = false;
                                _playerOp._curSongInfo = null;
                                $C.removeClassName($D.get("divplayer"), "m_player_playing")
                            },
                            0)
                        })
                    } catch(e) {}
                    if (singleFm.status) _playerOp._curSongInfo.curSongTime = ua.ie ? curtime: parseInt(curtime / 1E3);
                    var _h5 = !!ua.safari || ( !! ua.chrome || ( !! ua.isiPad || !!ua.isiPhone));
                    var _cur = _h5 ? parseInt(curtime / 1E3) : curtime;
                    var _elem = $D.get("qrc_ctn");
                    if (_playerOp._isPlaying) {
                        $D.get("ptime").innerHTML = common.formatTime(totaltime);
                        $D.get("spanplaybar").style.width = parseInt(_cur * 100 / totaltime, 10) + "%";
                        $D.get("spanprogress_op").style.left = parseInt(_cur * 100 / totaltime - 1, 10) + "%";
                        if (g_qrcHandlerChn.reqFlag) {
                            _elem.innerHTML = "<p>&nbsp;</p><p>\u6b4c\u8bcd\u6b63\u5728\u52a0\u8f7d\u4e2d...</p>";
                            return
                        } else {
                            var _innerHtml = !!_elem && ( !! _elem.getElementsByTagName && _elem.getElementsByTagName("p").length > 1) ? _elem.getElementsByTagName("p")[1].innerHTML: "";
                            if (g_qrcHandlerChn.haveqrc && (_innerHtml == "\u6b4c\u8bcd\u6b63\u5728\u52a0\u8f7d\u4e2d..." || _innerHtml == "")) {
                                _elem.innerHTML = '<p id="line_null" ><span class="current">&nbsp;</span></p>' + g_qrcHandlerChn.printQrcLines();
                                g_qrcHandlerChn.startAnimFrame("qrc_ctn");
                                return
                            } else if (!g_qrcHandlerChn.haveqrc && g_lrcHandlerChn.reqFlag == -1) {
                                common.isQusicSong(_playerOp._curSongInfo) ? (g_lrcHandlerChn.init(g_qrcHandlerChn.playingSong), _elem.innerHTML = "<p>&nbsp;</p><p>\u6b4c\u8bcd\u6b63\u5728\u52a0\u8f7d\u4e2d...</p>") : (g_lrcHandlerChn.haveLrc = false, g_lrcHandlerChn.reqFlag = 1);
                                return
                            } else if (!g_qrcHandlerChn.haveqrc && g_lrcHandlerChn.reqFlag == 0) return;
                            else if (!g_qrcHandlerChn.haveqrc && g_lrcHandlerChn.reqFlag == 1) if (g_lrcHandlerChn.haveLrc && (_innerHtml == "\u6b4c\u8bcd\u6b63\u5728\u52a0\u8f7d\u4e2d..." || _innerHtml == "")) {
                                var _lrcLines = g_lrcHandlerChn.printLrcLines();
                                if (_lrcLines == "") {
                                    g_lrcHandlerChn.haveLrc = false;
                                    _elem.innerHTML = '<p>&nbsp;</p><p><a href="http://support.qq.com/write.shtml?fid=602" target="_blank">\u8be5\u5355\u66f2\u6682\u65e0\u6b4c\u8bcd\uff0c\u70b9\u51fb\u8fd9\u91cc\u8fdb\u884c\u53cd\u9988</a></p>';
                                    return
                                }
                                _elem.innerHTML = '<p id="line_null"><span class="current">&nbsp;</span></p>' + _lrcLines;
                                g_lrcHandlerChn.startAnimFrame("qrc_ctn");
                                return
                            } else if (!g_lrcHandlerChn.haveLrc) {
                                _elem.innerHTML = '<p>&nbsp;</p><p><a href="http://support.qq.com/write.shtml?fid=602" target="_blank">\u8be5\u5355\u66f2\u6682\u65e0\u6b4c\u8bcd\uff0c\u70b9\u51fb\u8fd9\u91cc\u8fdb\u884c\u53cd\u9988</a></p>';
                                return
                            }
                        }
                        if (g_qrcHandlerChn.haveqrc) {
                            if (g_qrcHandlerChn.getQrcLineItemByPlayTime(_h5 ? curtime: curtime * 1E3)) {
                                var _context = g_qrcHandlerChn.lineItem.context,
                                _prePID = g_qrcHandlerChn.qrcLineIndex - 1 < 0 ? "null": g_qrcHandlerChn.qrcLineIndex - 1,
                                _pID = g_qrcHandlerChn.qrcLineIndex;
                                var _pNode = $D.get("line_" + _pID);
                                if ( !! _pNode) {
                                    var highlight = "";
                                    if (_h5) {
                                        highlight = g_qrcHandlerChn.getHighlightWords(_h5 ? curtime: curtime * 1E3);
                                        if (typeof highlight !== "undefined" && highlight != "") _pNode.innerHTML = _context.replace(highlight, '<span class="current">' + highlight + "</span>")
                                    } else {
                                        highlight = '<span class="current">' + _context + "</span>";
                                        _pNode.innerHTML = highlight
                                    }
                                    if (_pID > 0) {
                                        var _preNode = $D.get("line_" + _prePID),
                                        _preSpan = null;
                                        if ( !! _preNode) {
                                            _preSpan = _preNode.getElementsByTagName("span")[0];
                                            $C.hasClassName(_preSpan, "current") && $C.removeClassName(_preSpan, "current")
                                        }
                                    }
                                }
                            }
                        } else if (g_lrcHandlerChn.haveLrc) {
                            if (g_lrcHandlerChn.getLyricLineItemByPlayTime(g_lrcHandlerChn.ms2playTime(_h5 ? curtime: curtime * 1E3))) {
                                var _prePID = g_lrcHandlerChn.preLrcItemIndex == -1 ? "null": g_lrcHandlerChn.preLrcItemIndex,
                                _pID = g_lrcHandlerChn.lrcItemIndex;
                                var _pNode = $D.get("line_" + _pID);
                                if ( !! _pNode && _pID != _prePID) {
                                    _pNode.innerHTML = '<span class="current">' + g_lrcHandlerChn.lrcItem.context + "</span>";
                                    if (_pID > 0) {
                                        var _preNode = $D.get("line_" + _prePID),
                                        _preSpan = null;
                                        if ( !! _preNode) {
                                            _preSpan = _preNode.getElementsByTagName("span")[0];
                                            $C.hasClassName(_preSpan, "current") && $C.removeClassName(_preSpan, "current")
                                        }
                                    }
                                }
                            }
                        } else _elem.innerHTML = '<p>&nbsp;</p><p><a href="http://support.qq.com/write.shtml?fid=602" target="_blank">\u8be5\u5355\u66f2\u6682\u65e0\u6b4c\u8bcd\uff0c\u70b9\u51fb\u8fd9\u91cc\u8fdb\u884c\u53cd\u9988</a></p>'
                    }
                };
                g_webPlayer.OnPlaying = function() { ! singleFm.leaving && singleFm.play();
                    _setPlayBtnStatus("play", "g_play");
                    _playerOp._errorPlayId = []
                };
                g_webPlayer.OnPlayPause = function() { ! singleFm.leaving && singleFm.pause();
                    _setPlayBtnStatus("pause", "g_pause")
                };
                g_webPlayer.OnPlayError = function(songinfo, index) {
                    cs.p("g_webPlayer.OnPlayError 1");
                    if (g_webPlayer.isQusicSong(songinfo)) {
                        cs.p("g_webPlayer.OnPlayError 2");
                        MUSIC.OZ.errorSend(118, 4, {
                            8 : 100
                        });
                        songinfo.mcdn = "ws";
                        g_webPlayer.getWebPlayer().setPlayURL();
                        cs.p("g_webPlayer.OnPlayError 3")
                    } else {
                        var _e = $D.get("divsonglist");
                        if ( !! _e) {
                            var _es = _e.getElementsByTagName("li");
                            if (_es.length > index) $C.addClassName(_es[index], "play_wrong")
                        }
                        if (!_playerOp._errorPlayId[songinfo.playid]) {
                            _playerOp._errorPlayId[songinfo.playid] = 1;
                            g_webPlayer.nextPlayer()
                        }
                    }
                };
                g_webPlayer.OnSongDownloading = function(progress) {
                    var _e = MUSIC.dom.get("downloadbar");
                    if ( !! _e) _e.style.width = progress + "%"
                };
                _updateMode(_curSettings.mode);
                _playerOp.renderSong();
                _bindEvent()
            },
            setPlayBtnStatus: _setPlayBtnStatus,
            renderSong: function(music, mapMyfav) {
                var _tpl_song = ['<a %(albumhref) target="contentFrame" class="album_pic" title="%(malbum)">', '<img src="%(albumpic)" alt="%(malbum)" onerror="this.src=\'http://imgcache.qq.com/mediastyle/y/img/cover_mine_130.jpg\'" />', "</a>", '<div class="music_info_main">', '<p class="music_name" title="%(msong)"><span>%(msongCut)</span><a onclick="g_topPlayer.singleFm.open();" href="javascript:;" class="icon_radio">\u7535\u53f0</a></p>', '<p class="singer_name" title="%(msinger)">%(msinger)</p>', '<p class="play_date" id="ptime">%(time)</p>', '<p class="music_op" style="display:%(display_op);">', '<strong class="%(class_like)" title="%(title_like)" onclick="%(func_like)" name="myfav_%(mmid)" mid="%(mmid)"><span>\u6211\u559c\u6b22</span></strong>', '<strong class="%(class_share)" title="%(title_share)" onclick="%(func_share)"><span>\u5206\u4eab</span></strong>', "</p>", "</div>"].join(""),
                albumpic = !!music ? g_mid.init({
                    mid_flag: 0
                }).getAlbumPic({
                    id: music.malbumid,
                    mid: music.malbummid,
                    type: 90
                }) : g_mid.init({
                    mid_flag: 0
                }).getAlbumPic({
                    id: 0,
                    mid: "0",
                    type: 90
                });
                if (!music) {
                    _localSong.get(function(list) {
                        if (list.length > 0) {
                            music = list[0];
                            if ( !! music.malbumid || !!music.mmid) albumpic = g_mid.init({
                                mid_flag: 0
                            }).getAlbumPic({
                                id: music.malbumid,
                                mid: music.malbummid,
                                type: 68
                            })
                        }
                    });
                    music = {
                        msong: "\u97f3\u4e50\u4f60\u7684\u751f\u6d3b",
                        msinger: "QQ\u97f3\u4e50",
                        albumpic: albumpic,
                        albumurl: "javascript:;",
                        display_op: "none",
                        time: ""
                    }
                } else {
                    music.albumpic = albumpic;
                    if (music.mtype == "qqmusic") {
                        var _mid = music.malbummid,
                        _url = "http://y.qq.com/y/static/album/" + _mid.slice(_mid.length - 2, _mid.length - 1) + "/" + _mid.slice(_mid.length - 1, _mid.length) + "/" + _mid + ".html";
                        music.albumhref = "href=\"javascript:;\" onclick=\"window.frames['contentFrame'].location='" + _url + "'\""
                    } else music.albumhref = ""
                }
                music.msongCut = music.msong.length > 16 ? music.msong.slice(0, 16) + "\u2026\u2026": music.msong;
                var _gososo = parseInt(music.gososo, 10),
                _down = parseInt(music.mdownload, 10);
                if (_gososo != 0 || _down != 1) {
                    music.class_like = "btn_like_n";
                    music.title_like = "\u6682\u4e0d\u63d0\u4f9b\u6b64\u6b4c\u66f2\u670d\u52a1";
                    music.func_like = "MUSIC.event.cancelBubble();";
                    music.class_share = "btn_share_n";
                    music.title_share = "\u6682\u4e0d\u63d0\u4f9b\u6b64\u6b4c\u66f2\u670d\u52a1";
                    music.func_share = "MUSIC.event.cancelBubble();"
                } else {
                    if ( !! mapMyfav && mapMyfav[music.mmid]) {
                        music.class_like = "btn_liked";
                        music.title_like = "\u53d6\u6d88\u559c\u6b22"
                    } else {
                        music.class_like = "btn_like";
                        music.title_like = "\u559c\u6b22"
                    }
                    music.func_like = "g_topPlayer.like(g_topPlayer.getCurSongInfo(), this);MUSIC.event.cancelBubble();";
                    music.class_share = "btn_share";
                    music.title_share = "\u5206\u4eab";
                    music.func_share = "g_topPlayer.share(g_topPlayer.getCurSongInfo(), this);MUSIC.event.cancelBubble();"
                }
                $D.get("divsonginfo").innerHTML = _tpl_song.jstpl_format(music);
                $D.get("spanplaybar").style.width = "0%";
                $D.get("downloadbar").style.width = "0%";
                $D.get("spanprogress_op").style.left = "0%"
            },
            resetLikeIcon: function() {
                function _do(mapLike) {
                    var _es = MUSIC.dom.getElementsByClassName("btn_like", "strong", document);
                    MUSIC.object.each(_es,
                    function(_e) {
                        var mid = _e.getAttribute("mid");
                        if ( !! mapLike && mapLike[mid]) {
                            _e.className = "btn_liked";
                            _e.title = "\u53d6\u6d88\u559c\u6b22"
                        }
                    });
                    _es = MUSIC.dom.getElementsByClassName("btn_liked", "strong", document);
                    MUSIC.object.each(_es,
                    function(_e) {
                        var mid = _e.getAttribute("mid");
                        if (!mapLike || !mapLike[mid]) {
                            _e.className = "btn_like";
                            _e.title = "\u559c\u6b22"
                        }
                    })
                }
                var uin = g_user.getUin();
                if (uin < 10001) _do();
                else g_mymusicAlbumMod.Song.getMyfavMap(uin, _do)
            },
            updateSongLike: function(songmid, isLiked) {
                var _es = $D.getByName("myfav_" + songmid, "strong");
                MUSIC.object.each(_es,
                function(_e) {
                    if (isLiked && _e.className == "btn_like") {
                        _e.className = "btn_liked";
                        _e.title = "\u53d6\u6d88\u559c\u6b22"
                    }
                    if (!isLiked && _e.className == "btn_liked") {
                        _e.className = "btn_like";
                        _e.title = "\u559c\u6b22"
                    }
                })
            },
            songHover: function(e, type) {
                if (type == "show") $C.addClassName(e, "play_hover");
                else if (!$C.hasClassName(e, "class_fav")) $C.removeClassName(e, "play_hover")
            },
            play: function(obj, callback) {
                if (singleFm.status) {
                    singleFm.cutTab("playlist");
                    singleFm.leaving = true;
                    singleFm.pause();
                    singleFm.singOnPlayBegin();
                    singleFm.status = false
                }
                _playerOp._isFirst = false;
                _stopTime();
                $E.cancelBubble();
                function _play() {
                    var pos = 0;
                    if ( !! obj) pos = listFrame.getSelectPos(obj);
                    g_webPlayer.playAnyPos(pos);
                    if ( !! callback) callback()
                }
                if (_curTab == "playlist") listFrame.setPlayList(0, _play);
                else if (!obj) {
                    var uin = g_user.getUin();
                    g_mymusicAlbumMod.getList(uin,
                    function(list) {
                        for (var i = 0,
                        len = list.length; i < len; i++) if (common.isDirOk(list[i])) {
                            listFrame.setPlayList(list[i].dirid, _play);
                            return
                        }
                    })
                } else {
                    var dirid = obj.parentNode.parentNode.getAttribute("dirid");
                    listFrame.setPlayList(dirid, _play)
                }
                _setPlayBtnStatus("pause", "g_wait");
                pgvClickStat("playone")
            },
            trogglePlay: function() {
                var _e = $D.get("btnplay");
                if (_e.className == "play_bt") if (!_playerOp._isFirst) g_webPlayer.startPlayer();
                else _playerOp.play();
                else g_webPlayer.pausePlayer()
            },
            prev: function() {
                $E.cancelBubble();
                if (singleFm.status) {
                    if (singleFm.deling || (singleFm._delBtnStatus || singleFm._nexting)) return;
                    if (singleFm._preving) {
                        showTips("\u8bf7\u7a0d\u5019", "\u7cfb\u7edf\u6b63\u5728\u52aa\u529b\u5207\u6362\u6b4c\u66f2", 1, "width:190px");
                        if (singleFm._timeout) {
                            clearTimeout(singleFm._timeout);
                            singleFm._timeout = null
                        }
                        singleFm._timeout = setTimeout(function() {
                            hideTips();
                            singleFm._timeout = null
                        },
                        2E3);
                        return
                    }
                    var _curSong = $D.getElementsByClassName("current", "li", $D.get("single_radio_list"))[0],
                    _prev = $D.getPreviousSibling(_curSong);
                    if ($C.hasClassName(_prev, "first")) return;
                    singleFm.report(2, {
                        qq: g_user.getUin() || 0,
                        songid: _playerOp._curSongInfo.mid,
                        oper: 7,
                        rid: singleFm.rid,
                        sid: _playerOp._curSongInfo.mid,
                        time: _playerOp._curSongInfo.minterval,
                        playtime: _playerOp._curSongInfo.curSongTime
                    })
                }
                _stopTime();
                if (_curPlayDirid == -1) this.play(null,
                function() {
                    g_webPlayer.lastPlayer()
                });
                else g_webPlayer.lastPlayer();
                pgvClickStat("prevone")
            },
            next: function() {
                $E.cancelBubble();
                if (singleFm.status) {
                    if (singleFm.deling || (singleFm._delBtnStatus || singleFm._preving)) return;
                    if (singleFm.loadingData) {
                        showTips("\u8bf7\u7a0d\u5019", "\u7cfb\u7edf\u6b63\u5728\u52a0\u8f7d\u66f4\u591a\u6b4c\u66f2", 1, "width:170px");
                        if (singleFm._timeout) {
                            clearTimeout(singleFm._timeout);
                            singleFm._timeout = null
                        }
                        singleFm._timeout = setTimeout(function() {
                            hideTips();
                            singleFm._timeout = null
                        },
                        2E3);
                        return
                    }
                    if (singleFm._nexting) {
                        showTips("\u8bf7\u7a0d\u5019", "\u7cfb\u7edf\u6b63\u5728\u52aa\u529b\u5207\u6362\u6b4c\u66f2", 1, "width:190px");
                        if (singleFm._timeout) {
                            clearTimeout(singleFm._timeout);
                            singleFm._timeout = null
                        }
                        singleFm._timeout = setTimeout(function() {
                            hideTips();
                            singleFm._timeout = null
                        },
                        2E3);
                        return
                    }
                    var _curSong = $D.getElementsByClassName("current", "li", $D.get("single_radio_list"))[0],
                    _next = $D.getNextSibling(_curSong);
                    if (!_next || !$C.hasClassName(_next, "next")) return;
                    singleFm.report(2, {
                        qq: g_user.getUin() || 0,
                        songid: _playerOp._curSongInfo.mid,
                        oper: 7,
                        rid: singleFm.rid,
                        sid: _playerOp._curSongInfo.mid,
                        time: _playerOp._curSongInfo.minterval,
                        playtime: _playerOp._curSongInfo.curSongTime
                    })
                }
                _stopTime();
                if (_curPlayDirid == -1) this.play();
                else g_webPlayer.nextPlayer();
                pgvClickStat("nextone")
            },
            like: function(songInfo, self) {
                var uin = g_user.getUin();
                if (uin < 10001) {
                    if (/\/y\/static\/login.html/.test(top.window.frames["contentFrame"].location.href)) g_popup.show(1, "\u8bf7\u5148\u767b\u5f55", "", 3E3, 200);
                    else g_user.openLogin();
                    return
                }
                function _func(songinfo) {
                    if (self.className == "single_radio_like" || self.className == "btn_like") g_mymusicAlbumMod.Song.like(songinfo.mmid, 3,
                    function() {
                        _playerOp.updateSongLike(songinfo.mmid, 1);
                        Events.trigger("likeSong", songinfo.mid, 1);
                        pgvClickStat("like")
                    });
                    else g_mymusicAlbumMod.Song.unlike(songinfo.mmid, 3,
                    function() {
                        _playerOp.updateSongLike(songinfo.mmid, 0);
                        Events.trigger("likeSong", songinfo.mid, 0);
                        pgvClickStat("unlike")
                    })
                }
                _func(songInfo)
            },
            share: function(obj, curSongInfo) {
                var uin = g_user.getUin();
                if (uin < 10001) {
                    if (/\/y\/static\/login.html/.test(top.window.frames["contentFrame"].location.href)) g_popup.show(1, "\u8bf7\u5148\u767b\u5f55", "", 3E3, 200);
                    else g_user.openLogin();
                    return
                }
                if ( !! curSongInfo) {
                    g_trackServ.shareMusic(curSongInfo);
                    if (singleFm.status) singleFm.report(5, {
                        qq: g_user.getUin() || 0,
                        songid: curSongInfo.mid,
                        oper: 4,
                        rid: singleFm.rid,
                        sid: curSongInfo.mid,
                        time: curSongInfo.minterval,
                        playtime: _playerOp._curSongInfo.curSongTime
                    })
                } else listFrame.getSelectSongInfo(obj,
                function(songinfo) {
                    g_trackServ.shareMusic(songinfo);
                    if (singleFm.status) singleFm.report(5, {
                        qq: g_user.getUin() || 0,
                        songid: songinfo.mid,
                        oper: 4,
                        rid: singleFm.rid,
                        sid: songinfo.mid,
                        time: songinfo.minterval,
                        playtime: _playerOp._curSongInfo.curSongTime
                    })
                })
            },
            _objSong: null,
            fav: function(obj) {
                $E.cancelBubble();
                _hide();
                _playerOp._objSong = obj;
                function _show() {
                    var pos = $D.getPosition(_playerOp._objSong),
                    _ele = $D.get("divfavtips"),
                    _posList = $D.getPosition($D.get("divlistmain")),
                    _top = parseInt($D.get("divplaylist").style.top || 0),
                    _heigth = pos.bottom - _posList.top;
                    if (_posList.height - _heigth > 186) {
                        _ele.style.top = _heigth - _top + "px";
                        _ele.className = "pop_fav_music_1"
                    } else {
                        _ele.style.top = _heigth - _top - 200 + "px";
                        _ele.className = "pop_fav_music_2"
                    }
                    $C.addClassName(_playerOp._objSong, "play_hover class_fav");
                    showElement(_ele)
                }
                function _hide() {
                    if (_playerOp._objSong) {
                        $C.removeClassName(_playerOp._objSong, "play_hover class_fav");
                        hideElement("divfavtips");
                        _playerOp._objSong = null
                    }
                }
                function _click(event, ele) {
                    listFrame.getSelectSongInfo(_playerOp._objSong,
                    function(songinfo) {
                        _hide();
                        g_trackServ.fav(songinfo, {
                            dirid: ele.getAttribute("dirid")
                        })
                    })
                }
                var _e = MUSIC.dom.get("divfavtips");
                if (!_e) {
                    var uin = g_user.getUin();
                    if (uin < 10001) {
                        if (/\/y\/static\/login.html/.test(top.window.frames["contentFrame"].location.href)) g_popup.show(1, "\u8bf7\u5148\u767b\u5f55", "", 3E3, 200);
                        else g_user.openLogin();
                        return
                    }
                    g_mymusicAlbumMod.getList(uin,
                    function(list) {
                        var _tpl1 = ['<div class="pop_arrow" style="right:14px;"></div>', '<div class="pop_fav_music_frame">', "<h5>\u6536\u85cf\u5230\u6b4c\u5355\uff1a</h5>", '<div class="pop_fav_music_main" id="pop_fav_music_main">', '<ul id="favlistdiv">%(dir_list)</ul>', '<div class="pop_scroll" style="display:%(display_scroll);"><span class="pop_scrolling" style="height:60%;top:10%;" id="favscrollbar"></span></div>', "</div>", "</div>"].join(""),
                        _tpl2 = '<li dirid="%(dirid)" title="%(dirname)"><a href="javascript:;">%(dirname)</a></li>';
                        var arr = [];
                        for (var i = 0,
                        len = list.length; i < len; i++) {
                            var dirinfo = list[i];
                            if (uin != dirinfo.owner.uin || dirinfo.dirid >= 201 && dirinfo.dirid <= 210) continue;
                            dirinfo.index = i;
                            arr.push(_tpl2.jstpl_format(dirinfo))
                        }
                        _e = $D.createElementIn("div", $D.get("divsonglist"), null, {
                            "id": "divfavtips",
                            "style": "left:379px;"
                        });
                        _e.innerHTML = _tpl1.jstpl_format({
                            dir_list: arr.join(""),
                            display_scroll: arr.length > 5 ? "": "none"
                        });
                        var _es = _e.getElementsByTagName("li");
                        MUSIC.object.each(_es,
                        function(e1) {
                            $E.on(e1, "click", _click, e1)
                        });
                        $E.on(_e, "click",
                        function() {
                            $E.cancelBubble()
                        });
                        $E.on($D.get("divplayer"), "click", _hide);
                        setTimeout(_show, 50);
                        _favlistBar = new MUSIC.scrollbar;
                        _favlistBar.init({
                            barid: "favscrollbar",
                            contid: "favlistdiv",
                            ulid: "favlistdiv",
                            listbox: "pop_fav_music_main"
                        })
                    })
                } else setTimeout(_show, 50)
            },
            add: function(newlist, isPlay) {
                $E.cancelBubble();
                singleFm.cutTab("playList");
                if (isPlay) if (singleFm.status) {
                    singleFm.pause();
                    singleFm.leaving = true;
                    singleFm.singOnPlayBegin()
                }
                for (var i = newlist.length - 1; i >= 0; i--) if (newlist[i].msongurl && (!/(\.m4a)|(\.mp3)|(\&#46;m4a)|(\&#46;mp3)/i.test(newlist[i].msongurl) && !/(share\.weiyun\.qq\.com)|(share\&#46;weiyun\&#46;qq\&#46;com)/i.test(newlist[i].msongurl))) {
                    g_popup.show(1, "\u6b4c\u66f2\u201c" + newlist[i].msong + "\u201d\u64ad\u653e\u5931\u8d25\uff01", "\u5f88\u62b1\u6b49\uff0cQQ\u97f3\u4e50\u6682\u4e0d\u63d0\u4f9b\u6b64\u683c\u5f0f\u7684\u6b4c\u66f2\u670d\u52a1\u3002", "", 5E3, 360);
                    newlist.splice(i, 1)
                }
                _localSong.get(function(list) {
                    var oldlen = list.length,
                    addlen = newlist.length,
                    newlen = _localSong.add(newlist);
                    if (addlen == 0) return;
                    if (oldlen >= 300) {
                        showTips("\u64ad\u653e\u5217\u8868\u8d85\u8fc7300\u9996\u4e0a\u9650\uff01", "\u8bf7\u6e05\u7406\u5217\u8868\u4e2d\u4e0d\u9700\u8981\u7684\u6b4c\u66f2\u3002");
                        return
                    }
                    if (newlen <= oldlen) {
                        showTips("\u6dfb\u52a0\u6b4c\u66f2\u5931\u8d25\uff01", "\u8d85\u8fc7\u5bb9\u91cf\u4e0a\u9650\uff0c\u672c\u5730\u7535\u8111\u6700\u591a\u53ef\u4ee5\u7f13\u5b58</br>32k\u7684\u6b4c\u66f2\uff0c\u8bf7\u5220\u9664\u64ad\u653e\u5217\u8868\u5185\u7684\u6b4c\u66f2\u3002", 2);
                        return
                    }
                    if (oldlen + addlen > newlen) {
                        if (newlen == 300) showTips("\u64ad\u653e\u5217\u8868\u8d85\u8fc7300\u9996\u4e0a\u9650\uff01", "\u8bf7\u6e05\u7406\u5217\u8868\u4e2d\u4e0d\u9700\u8981\u7684\u6b4c\u66f2\u3002");
                        else showTips("\u6dfb\u52a0\u6b4c\u66f2\u5931\u8d25\uff01", "\u8d85\u8fc7\u5bb9\u91cf\u4e0a\u9650\uff0c\u672c\u5730\u7535\u8111\u6700\u591a\u53ef\u4ee5\u7f13\u5b58</br>32k\u7684\u6b4c\u66f2\uff0c\u8bf7\u5220\u9664\u64ad\u653e\u5217\u8868\u5185\u7684\u6b4c\u66f2\u3002", 2);
                        newlist = newlist.slice(0, newlen - oldlen)
                    }
                    var _e = $D.get("spanaddtips");
                    _e.style.top = "0px";
                    showElement(_e);
                    MUSIC.effect.run(_e, {
                        top: "-30"
                    },
                    {
                        duration: 1E3,
                        complete: function() {
                            setTimeout(function() {
                                hideElement(_e)
                            },
                            500)
                        }
                    });
                    listFrame.addSongHtml(newlist);
                    if (_curPlayDirid == 0) g_webPlayer.addSong(newlist, isPlay);
                    else if (isPlay) listFrame.setPlayList(0,
                    function() {
                        g_webPlayer.playAnyPos(oldlen)
                    });
                    _updateSongNum();
                    if ( !! g_msPinnedSite && !!g_external) g_msPinnedSite.initNewList();
                    _updateListScroll()
                })
            },
            del: function(obj) {
                $E.cancelBubble();
                var pos = listFrame.getSelectPos(obj),
                dirid = obj.parentNode.parentNode.getAttribute("dirid");
                if (dirid == 201) {
                    this.like(obj);
                    return
                }
                var _e = $D.get("divsonglist"),
                _list_len = !!_e ? _e.getElementsByTagName("li").length: 1;
                if (_list_len == 1 || !!_curPlayElem && pos == listFrame.getSelectPos(_curPlayElem)) {
                    $D.get("qrc_ctn").innerHTML = "";
                    if (g_qrcHandlerChn.haveqrc) g_qrcHandlerChn.clearQrcInfo();
                    else if (g_lrcHandlerChn.haveLrc) g_lrcHandlerChn.clearLrcInfo();
                    if (_list_len == 1) {
                        _playProgressBar.destroy();
                        _setPlayBtnStatus("pause", "g_first_play")
                    } else $D.get("spanprogress_op").style.left = "0%"
                }
                $D.removeElement(obj);
                _playlistBar.refresh();
                if (dirid == 0) {
                    _localSong.del(pos);
                    _updateSongNum()
                } else {
                    var uin = g_user.getUin();
                    g_mymusicAlbumMod.delSong2Dir({
                        uin: uin,
                        pos: pos,
                        dirid: dirid
                    })
                }
                if (dirid == _curPlayDirid) if (pos >= 0 && !g_webPlayer.delSong(pos)) {
                    _playerOp.renderSong();
                    _playerOp._isPlaying = false;
                    $C.removeClassName($D.get("divplayer"), "m_player_playing");
                    g_webPlayer.stopPlayer()
                }
                if ( !! g_msPinnedSite && !!g_external) g_msPinnedSite.initNewList()
            },
            insert: function(obj, songinfo) {
                $E.cancelBubble();
                var pos = listFrame.getSelectPos(obj),
                dirid = obj.parentNode.parentNode.getAttribute("dirid");
                if (dirid != 0) return;
                _localSong.get(function(list) {
                    var oldlen = list.length,
                    addlen = 1,
                    newlen = _localSong.insert(pos, songinfo);
                    if (oldlen >= 300) {
                        showTips("\u64ad\u653e\u5217\u8868\u8d85\u8fc7300\u9996\u4e0a\u9650\uff01", "\u8bf7\u6e05\u7406\u5217\u8868\u4e2d\u4e0d\u9700\u8981\u7684\u6b4c\u66f2\u3002");
                        return
                    }
                    if (newlen <= oldlen) {
                        showTips("\u6dfb\u52a0\u6b4c\u66f2\u5931\u8d25\uff01", "\u8d85\u8fc7\u5bb9\u91cf\u4e0a\u9650\uff0c\u672c\u5730\u7535\u8111\u6700\u591a\u53ef\u4ee5\u7f13\u5b58</br>32k\u7684\u6b4c\u66f2\uff0c\u8bf7\u5220\u9664\u64ad\u653e\u5217\u8868\u5185\u7684\u6b4c\u66f2\u3002", 2);
                        return
                    }
                    if (oldlen + addlen > newlen) if (newlen == 300) showTips("\u64ad\u653e\u5217\u8868\u8d85\u8fc7300\u9996\u4e0a\u9650\uff01", "\u8bf7\u6e05\u7406\u5217\u8868\u4e2d\u4e0d\u9700\u8981\u7684\u6b4c\u66f2\u3002");
                    else showTips("\u6dfb\u52a0\u6b4c\u66f2\u5931\u8d25\uff01", "\u8d85\u8fc7\u5bb9\u91cf\u4e0a\u9650\uff0c\u672c\u5730\u7535\u8111\u6700\u591a\u53ef\u4ee5\u7f13\u5b58</br>32k\u7684\u6b4c\u66f2\uff0c\u8bf7\u5220\u9664\u64ad\u653e\u5217\u8868\u5185\u7684\u6b4c\u66f2\u3002", 2);
                    var uin = g_user.getUin();
                    if (uin < 10001) $D.insertAdjacent(obj, 0, listFrame.getSongListHtml([songinfo]));
                    else g_mymusicAlbumMod.Song.getMyfavMap(uin,
                    function(mapMyfav) {
                        $D.insertAdjacent(obj, 0, listFrame.getSongListHtml([songinfo], mapMyfav))
                    });
                    var _e = $D.get("spanaddtips");
                    _e.style.top = "0px";
                    showElement(_e);
                    MUSIC.effect.run(_e, {
                        top: "-30"
                    },
                    {
                        duration: 1E3,
                        complete: function() {
                            setTimeout(function() {
                                hideElement(_e)
                            },
                            500)
                        }
                    });
                    g_webPlayer.insertSong(pos, songinfo);
                    _updateSongNum();
                    if ( !! g_msPinnedSite && !!g_external) g_msPinnedSite.initNewList();
                    _playlistBar.refresh()
                })
            },
            clearList: function() {
                _localSong.clear();
                var _es = $D.get("divsonglist").getElementsByTagName("ul");
                if (_es.length > 0) _es[0].innerHTML = "";
                _updateListScroll();
                _updateSongNum();
                if (!singleFm.status) {
                    $D.get("qrc_ctn").innerHTML = "";
                    if (g_qrcHandlerChn.haveqrc) g_qrcHandlerChn.clearQrcInfo();
                    else if (g_lrcHandlerChn.haveLrc) g_lrcHandlerChn.clearLrcInfo();
                    _playProgressBar.destroy();
                    _setPlayBtnStatus("pause", "g_first_play");
                    _playerOp.renderSong();
                    _playerOp._isPlaying = false;
                    _playerOp._curSongInfo = null;
                    $C.removeClassName($D.get("divplayer"), "m_player_playing");
                    g_webPlayer.clearPlayerList();
                    if ( !! g_msPinnedSite && !!g_external) {
                        g_msPinnedSite.paramsConfig.isPlaying = false;
                        g_msPinnedSite.paramsConfig.isPaused = false;
                        g_msPinnedSite.trogglePlayPauseBtn();
                        g_msPinnedSite.initNewList()
                    }
                    _setQQstatus(true)
                }
            },
            setPlayWay: function() {
                _stopTime();
                $E.cancelBubble();
                var _list = ['<strong title="\u968f\u673a\u64ad\u653e" class="unordered_bt" onclick="g_topPlayer.realSetPlayWay(4);"><span>\u968f\u673a\u64ad\u653e</span></strong>', '<strong title="\u5355\u66f2\u5faa\u73af" class="cycle_single_bt" onclick="g_topPlayer.realSetPlayWay(1);"><span>\u5355\u66f2\u5faa\u73af</span></strong>', '<strong title="\u5217\u8868\u5faa\u73af" class="cycle_bt" onclick="g_topPlayer.realSetPlayWay(3);"><span>\u5217\u8868\u5faa\u73af</span></strong>', '<strong title="\u987a\u5e8f\u64ad\u653e" class="ordered_bt" onclick="g_topPlayer.realSetPlayWay(2);"><span>\u987a\u5e8f\u64ad\u653e</span></strong>'],
                _map = [1, 3, 2, 0],
                _length = _map.length;
                var _start = _map[_curSettings.mode - 1] + 1,
                _tmp = [];
                for (var i = 0; i < _length; i++) {
                    var index = (i + _start) % _length;
                    _tmp.push(_list[index])
                }
                var _e = $D.get("divselect");
                _e.innerHTML = _tmp.join("");
                $E.addEvent(document, "click", _playerOp.realSetPlayWay);
                showElement(_e)
            },
            realSetPlayWay: function(mode) {
                $E.cancelBubble();
                var _e = $D.get("divselect");
                hideElement(_e);
                if (mode && mode > 0) _updateMode(mode)
            },
            getCurSongInfo: function() {
                return _playerOp._curSongInfo
            }
        }
    } ();
    var _volumeMod = function() {
        var _oBar, _oOp, _oVolume, isFirst = true;
        function init() {
            if (!isFirst) return;
            isFirst = false;
            var vol = g_webPlayer.getVolumn();
            if (vol <= 0 || vol > 100) vol = 75;
            _oBar = $D.get("spanvolumebar");
            _oOp = $D.get("spanvolumeop");
            _oVolume = $D.get("spanvolume");
            _oMute = $D.get("spanmute");
            _update(vol);
            $E.addEvent(_oVolume, "click",
            function() {
                MUSIC.event.cancelBubble();
                MUSIC.event.preventDefault();
                var _pos = $D.getPosition(_oVolume),
                _mousex = $E.mouseX();
                _change_vol = parseInt((_mousex - _pos["left"]) * 100 / _pos["width"], 10);
                _update(_change_vol)
            });
            $E.addEvent(_oMute, "click", _mute);
            $E.addEvent(_oOp, "mousedown",
            function() {
                MUSIC.event.cancelBubble();
                MUSIC.event.preventDefault();
                var body = document.body;
                MUSIC.event.addEvent(body, "mousemove", _move);
                MUSIC.event.addEvent(body, "mouseup",
                function() {
                    MUSIC.event.removeEvent(body, "mousemove", _move);
                    $C.removeClassName(_oVolume.parentNode, "adjust_volume")
                });
                var _divplayer = $D.get("divplayer");
                $E.addEvent(body, "mouseout",
                function(e) {
                    var _target = $E.getTarget(e);
                    if ( !! _target && !$D.isAncestor(_divplayer, _target)) {
                        $E.removeEvent(body, "mousemove", _move);
                        $C.removeClassName(_oVolume.parentNode, "adjust_volume")
                    }
                })
            })
        }
        function _mute() {
            MUSIC.event.cancelBubble();
            MUSIC.event.preventDefault();
            if (isFirst) return;
            if (_oMute.className == "volume_icon") {
                _oMute.className = "volume_mute";
                _oMute.title = "\u70b9\u51fb\u5f00\u542f\u58f0\u97f3(M)"
            } else {
                _oMute.className = "volume_icon";
                _oMute.title = "\u70b9\u51fb\u8bbe\u4e3a\u9759\u97f3(M)"
            }
            g_webPlayer.mutePlayer()
        }
        function _move() {
            MUSIC.event.cancelBubble();
            MUSIC.event.preventDefault();
            var _pos = $D.getPosition(_oVolume),
            _mousex = $E.mouseX();
            _change_vol = parseInt((_mousex - _pos["left"]) * 100 / _pos["width"], 10);
            $C.addClassName(_oVolume.parentNode, "adjust_volume");
            _update(_change_vol)
        }
        function _update(vol) {
            if (isFirst) return;
            if (!isNaN(vol) && (vol >= 0 && vol < 101)) {
                _oBar.style.width = vol + "%";
                _oOp.style.left = vol + "%";
                g_webPlayer.setVolumn(vol);
                if (_oMute.className != "volume_icon") {
                    _oMute.className = "volume_icon";
                    _oMute.title = "\u70b9\u51fb\u8bbe\u4e3a\u9759\u97f3(M)";
                    g_webPlayer.mutePlayer()
                }
            }
        }
        function _incVol() {
            var vol = g_webPlayer.getVolumn();
            if (vol < 0 || vol > 100) vol = 75;
            vol += 1;
            _update(vol)
        }
        function _decVol() {
            var vol = g_webPlayer.getVolumn();
            if (vol < 0 || vol > 100) vol = 75;
            vol -= 1;
            _update(vol)
        }
        return {
            init: init,
            incVol: _incVol,
            decVol: _decVol,
            mute: _mute
        }
    } ();
    var _playProgressBar = function() {
        var _bgBar, _oOp, _oProgress, _opMax, _timeShow = null,
        _downloadBar;
        function init() {
            _bgBar = $D.get("spanplayer_bgbar");
            _downloadBar = $D.get("downloadbar");
            _oOp = $D.get("spanprogress_op");
            _oProgress = $D.get("spanplaybar");
            _timeShow = $D.get("time_show");
            _opMax = $D.getSize(_bgBar)[0] - $D.getSize(_oOp)[0] - 2 * $D.getPosition(_oOp)["left"] - 1;
            $E.addEvent(_bgBar, "click",
            function() {
                MUSIC.event.cancelBubble();
                MUSIC.event.preventDefault();
                var _pos = $D.getPosition(_bgBar),
                _mousex = $E.mouseX(),
                _change_pos = parseInt((_mousex - _pos["left"]) * 100 / _pos["width"], 10);
                _update(_change_pos)
            });
            $E.addEvent(_downloadBar, "click",
            function() {
                MUSIC.event.cancelBubble();
                MUSIC.event.preventDefault();
                var _pos = $D.getPosition(_downloadBar),
                _mousex = $E.mouseX(),
                _change_pos = parseInt((_mousex - _pos["left"]) * 100 / _pos["width"], 10);
                _update(_change_pos)
            });
            $E.addEvent(_oProgress, "click",
            function() {
                MUSIC.event.cancelBubble();
                MUSIC.event.preventDefault();
                var _pos = $D.getPosition(_bgBar),
                _mousex = $E.mouseX(),
                _change_pos = parseInt((_mousex - _pos["left"]) * 100 / _pos["width"], 10);
                _update(_change_pos)
            });
            $E.addEvent(_oOp, "mousedown",
            function(e) {
                $E.cancelBubble();
                $E.preventDefault($E.getEvent(e));
                var _startMove = (new Date).getTime(),
                opxy = $D.getXY(_oOp),
                ex = $E.mouseX(e),
                left = opxy[0] - $D.getXY(_bgBar)[0],
                arr = {
                    "left": left,
                    "ex": ex
                },
                body = document.body;
                $E.addEvent(body, "mousemove",
                function(e) {
                    var now = (new Date).getTime();
                    if (now - _startMove < 300) return false;
                    _move(e, arr)
                },
                arr);
                $E.addEvent(body, "mouseup",
                function(e) {
                    $E.removeEvent(body, "mousemove")
                });
                var _divplayer = $D.get("divplayer");
                $E.addEvent(body, "mouseout",
                function(e) {
                    var _target = $E.getTarget(e);
                    if ( !! _target && !$D.isAncestor(_divplayer, _target)) $E.removeEvent(body, "mousemove")
                })
            });
            if ( !! _timeShow) {
                $E.addEvent(_bgBar.parentNode, "mousemove",
                function(e) {
                    if ( !! g_topPlayer.getCurSongInfo()) {
                        var _totaltime = g_webPlayer.getSongDuration();
                        var _pos = $D.getPosition(_bgBar.parentNode),
                        _mousex = $E.mouseX(),
                        _change_pos = parseInt((_mousex - _pos["left"]) * 1E4 / _pos["width"], 10);
                        var showTime = parseInt(_change_pos * _totaltime / 1E4, 10);
                        _timeShow.innerHTML = common.formatTime(showTime);
                        MUSIC.dom.setStyle(_timeShow.parentNode, "left", parseInt(_mousex - 22, 10) + "px");
                        showElement(_timeShow.parentNode)
                    }
                });
                $E.addEvent(_bgBar.parentNode, "mouseout",
                function(e) {
                    hideElement(_timeShow.parentNode)
                })
            }
        }
        function _move(evt, arr) {
            $E.cancelBubble();
            $E.preventDefault(evt);
            var _evt = $E.getEvent(evt),
            _x = $E.mouseX(_evt);
            var moveX = _x - arr.ex;
            var newX = moveX + arr.left,
            _pos = $D.getPosition(_bgBar);
            newX = newX <= _pos["left"] ? _pos["left"] : newX > _opMax ? _opMax: newX;
            var _change_pos = parseInt((newX - _pos["left"]) * 100 / _pos["width"], 10);
            _update(_change_pos)
        }
        function _update(pos) {
            if (!isNaN(pos) && (pos >= 0 && pos < 101)) {
                var _downProgress = g_webPlayer.getDownloadProgress();
                pos = Math.min(pos, _downProgress);
                _oProgress.style.width = pos + "%";
                _oOp.style.left = pos - 1 + "%";
                var _totaltime = g_webPlayer.getSongDuration(),
                _change_time_pos = pos * _totaltime / 100;
                var lCurPos = parseInt(_change_time_pos);
                if (!_playerOp._isPlaying) g_topPlayer.play(_curPlayElem,
                function() {
                    setTimeout(function() {
                        g_webPlayer.setPlayProgress(lCurPos);
                        _toCurLyricLine(lCurPos)
                    },
                    500)
                });
                else {
                    g_webPlayer.setPlayProgress(lCurPos);
                    _toCurLyricLine(lCurPos)
                }
            }
        }
        function _toCurLyricLine(lCurPos) {
            var _h5 = !!ua.safari || ( !! ua.chrome || ( !! ua.isiPad || !!ua.isiPhone));
            if (g_qrcHandlerChn.haveqrc) {
                var _node = $D.get("line_" + g_qrcHandlerChn.qrcLineIndex);
                if ( !! _node && !!_node.getElementsByTagName) if ($C.hasClassName(_node.getElementsByTagName("span")[0], "current")) $C.removeClassName(_node.getElementsByTagName("span")[0], "current");
                if (g_qrcHandlerChn.getQrcLineItemByPlayTime(parseInt(lCurPos * 1E3))) {
                    var _idx = g_qrcHandlerChn.qrcLineIndex,
                    heights = 0;
                    while (_idx-->0) {
                        var _n = $D.get("line_" + _idx);
                        heights += !!_n ? $D.get("line_" + _idx).offsetHeight: 24
                    }
                    $D.get("qrc_ctn").scrollTop = heights;
                    var _context = g_qrcHandlerChn.lineItem.context,
                    _pID = g_qrcHandlerChn.qrcLineIndex;
                    var _pNode = $D.get("line_" + _pID);
                    if ( !! _pNode) {
                        var highlight = "";
                        if (_h5) {
                            highlight = g_qrcHandlerChn.getHighlightWords(parseInt(lCurPos * 1E3));
                            if (typeof highlight !== "undefined" && highlight != "") _pNode.innerHTML = _context.replace(highlight, '<span class="current">' + highlight + "</span>")
                        } else {
                            highlight = '<span class="current">' + _context + "</span>";
                            _pNode.innerHTML = highlight
                        }
                        if (_pID > 0) {
                            var _preNode = $D.get("line_" + _prePID),
                            _preSpan = null;
                            if ( !! _preNode) {
                                _preSpan = _preNode.getElementsByTagName("span")[0];
                                $C.hasClassName(_preSpan, "current") && $C.removeClassName(_preSpan, "current")
                            }
                        }
                    }
                    g_qrcHandlerChn.restartAnimFrame("qrc_ctn")
                }
            } else if (g_lrcHandlerChn.haveLrc) {
                var _node = $D.get("line_" + g_lrcHandlerChn.lrcItemIndex);
                if ( !! _node && !!_node.getElementsByTagName) if ($C.hasClassName(_node.getElementsByTagName("span")[0], "current")) $C.removeClassName(_node.getElementsByTagName("span")[0], "current");
                if (g_lrcHandlerChn.getLyricLineItemByPlayTime(g_lrcHandlerChn.ms2playTime(parseInt(lCurPos * 1E3)))) {
                    var _idx = g_lrcHandlerChn.lrcItemIndex,
                    heights = 0;
                    while (_idx-->0) {
                        var _n = $D.get("line_" + _idx);
                        heights += !!_n ? $D.get("line_" + _idx).offsetHeight: 24
                    }
                    $D.get("qrc_ctn").scrollTop = heights;
                    var _prePID = g_lrcHandlerChn.preLrcItemIndex == -1 ? "null": g_lrcHandlerChn.preLrcItemIndex,
                    _pID = g_lrcHandlerChn.lrcItemIndex;
                    var _pNode = $D.get("line_" + _pID);
                    if ( !! _pNode) {
                        _pNode.innerHTML = '<span class="current">' + g_lrcHandlerChn.lrcItem.context + "</span>";
                        if (_pID > 0) {
                            var _preNode = $D.get("line_" + _prePID),
                            _preSpan = null;
                            if ( !! _preNode) {
                                _preSpan = _preNode.getElementsByTagName("span")[0];
                                $C.hasClassName(_preSpan, "current") && $C.removeClassName(_preSpan, "current")
                            }
                        }
                    }
                    g_lrcHandlerChn.restartAnimFrame("qrc_ctn")
                }
            }
        }
        function destroy() {
            if ( !! _oOp) _oOp.style.left = "0%";
            MUSIC.event.removeEvent(_bgBar, "click");
            MUSIC.event.removeEvent(_oProgress, "click");
            MUSIC.event.removeEvent(_oOp, "mousedown")
        }
        return {
            init: init,
            destroy: destroy
        }
    } ();
    function _updateSongNum() {
        _localSong.get(function(list) {
            MUSIC.dom.get("spansongnum1").innerHTML = "<span>" + list.length + "</span>";
            if (list.length > 0) {
                hideElement("divnulllist");
                showElement("divplaylist");
                _playListEmpty = false
            } else {
                hideElement("divplaylist");
                showElement("divnulllist");
                _playListEmpty = true
            }
        })
    }
    function showPlaylist() {
        $E.cancelBubble();
        MUSIC.effect.show("divplayframe", 300,
        function() {
            if (ua.ie == 6 && singleFm.status) setTimeout(function() {
                singleFm.cutTab()
            },
            50)
        });
        if (_isFirstShow) {
            _playlistBar = new MUSIC.scrollbar;
            _playlistBar.init({
                barid: "spanbar",
                contid: "divplaylist",
                bar_hover: "play_list_scrolling",
                ulid: "divsonglist",
                listbox: "divlistmain"
            });
            _isFirstShow = false;
            $E.addEvent($D.get("divplayer"), "click",
            function() {
                $E.cancelBubble()
            });
            $E.addEvent(document, "click",
            function() {
                hidePlaylist()
            });
            $E.addEvent($D.get("divsongframe"), "click",
            function() {
                hidePlaylist()
            })
        } else _updateListScroll()
    }
    function hidePlaylist(callback) {
        var _e = MUSIC.dom.get("divplayframe");
        if (_e.style.display == "none") return;
        MUSIC.effect.hide("divplayframe", 300,
        function() {
            if (MUSIC.lang.isFunction(callback)) callback();
            showElement("spansongnum1")
        })
    }
    function trogglePlayer(type) {
        _stopTime();
        hideTips();
        var _ePlayer = $D.get("divplayer"),
        _target = $D.get("btnfold");
        if (type != "show" && type != "hide") type = _target.title == "\u70b9\u51fb\u5c55\u5f00" ? "show": "hide";
        if (type == "show") {
            $C.removeClassName(_ePlayer, "m_player_playing");
            MUSIC.effect.run(_ePlayer, {
                left: "+0"
            },
            {
                duration: 600,
                complete: function() {
                    _target.title = "\u70b9\u51fb\u6536\u8d77";
                    $C.removeClassName(_ePlayer, "m_player_folded")
                }
            })
        } else {
            hideElement("divlrctips");
            hideElement("addSongTips");
            function _func() {
                MUSIC.effect.run(_ePlayer, {
                    left: "-540"
                },
                {
                    duration: 600,
                    complete: function() {
                        _target.title = "\u70b9\u51fb\u5c55\u5f00";
                        $C.addClassName(_ePlayer, "m_player_folded");
                        if (_playerOp._isPlaying) $C.addClassName(_ePlayer, "m_player_playing")
                    }
                })
            }
            if ($D.get("divplayframe").style.display == "none") _func();
            else hidePlaylist(_func)
        }
    }
    function troggleLrcPannel() {
        var _btn = $D.get("btnlrc"),
        _pannel = $D.get("player_lyrics_pannel");
        if ($C.hasClassName(_btn, "btn_lyrics_enabled")) {
            if (g_qrcHandlerChn.haveqrc && !!g_qrcHandlerChn.aniFrame) g_qrcHandlerChn.stopAnimFrame();
            else if (g_lrcHandlerChn.haveLrc && !!g_lrcHandlerChn.aniFrame) g_lrcHandlerChn.stopAnimFrame();
            _pannel.style.display = "none";
            $C.removeClassName(_btn, "btn_lyrics_enabled");
            $C.addClassName(_btn, "btn_lyrics_disabled");
            _btn.title = "\u663e\u793a\u6b4c\u8bcd(L)"
        } else if ($C.hasClassName(_btn, "btn_lyrics_disabled")) {
            _pannel.style.display = "block";
            var _lrc = $D.get("qrc_ctn"),
            _heights1 = 0,
            _top1 = 0,
            _idx1 = g_qrcHandlerChn.qrcLineIndex;
            _heights2 = 0,
            _top2 = 0,
            _idx2 = g_lrcHandlerChn.lrcItemIndex,
            _max = _lrc && _lrc.scrollHeight - 72;
            if (g_qrcHandlerChn.haveqrc) {
                while (_idx1-->0) _heights1 += $D.get("line_" + _idx1).offsetHeight;
                _top1 = _heights1;
                _lrc.scrollTop = _top1 >= _max ? _max: _top1;
                g_qrcHandlerChn.startAnimFrame("qrc_ctn")
            } else if (g_lrcHandlerChn.haveLrc) {
                while (_idx2-->0) _heights2 += $D.get("line_" + _idx2).offsetHeight;
                _top2 = _heights2;
                _lrc.scrollTop = _top2 >= _max ? _max: _top2;
                g_lrcHandlerChn.startAnimFrame("qrc_ctn")
            }
            $C.removeClassName(_btn, "btn_lyrics_disabled");
            $C.addClassName(_btn, "btn_lyrics_enabled");
            _btn.title = "\u9690\u85cf\u6b4c\u8bcd(L)"
        }
    }
    function showDragTips() {
        if ( !! top["drag_songinfo"] && top["drag_songinfo"].trim() != "") {
            var _tips = $D.get("drag_tips");
            if ( !! _tips) {
                var _songinfo = g_trackServ.formatMusic(top["drag_songinfo"]);
                _tips.innerHTML = _songinfo.msong + " - " + _songinfo.msinger;
                _tips = _tips.parentNode;
                $E.addEvent($D.get("divplayer"), "dragover",
                function(e) {
                    var _x = $E.mouseX(e),
                    _y = $E.mouseY(e) - 25;
                    $D.setXY(_tips, _x, _y)
                });
                $E.addEvent(frames["contentFrame"].document, "dragover",
                function(e) {
                    var _x = $E.mouseX(e),
                    _y = $E.mouseY(e) + 100;
                    $D.setXY(_tips, _x, _y)
                });
                showElement(_tips)
            } else {
                _tips = $D.createElementIn("div", null, false, {
                    style: "display:none;"
                });
                _tips.className = "list_choose";
                var _songinfo = g_trackServ.formatMusic(top["drag_songinfo"]);
                _tips.innerHTML = ['<strong class="music_name" id="drag_tips">', _songinfo.msong, " - ", _songinfo.msinger, "</strong>"].join("");
                $E.addEvent($D.get("divplayer"), "dragover",
                function(e) {
                    var _x = $E.mouseX(e),
                    _y = $E.mouseY(e) - 25;
                    $D.setXY(_tips, _x, _y)
                });
                $E.addEvent(frames["contentFrame"].document, "dragover",
                function(e) {
                    var _x = $E.mouseX(e),
                    _y = $E.mouseY(e) + 100;
                    $D.setXY(_tips, _x, _y)
                });
                showElement(_tips)
            }
        }
    }
    function hideDragTips() {
        var _tips = $D.get("drag_tips");
        if ( !! _tips) {
            _tips = _tips.parentNode;
            hideElement(_tips);
            $E.removeEvent(document, "dragover");
            $E.removeEvent(frames["contentFrame"].document, "dragover")
        }
    }
    var singleFm = function() {
        var $extend = MUSIC.object.extend,
        _emptyStatus = false,
        _timeout = null,
        _tabContainer = $D.get("tab_container"),
        _playListTab = $D.get("playlist_tab"),
        _fmTab = $D.get("fm_tab"),
        _emptyList = $D.get("divnullFMlist"),
        _divnulllist = $D.get("divnulllist"),
        _FMEle = $D.get("FM_container"),
        _playlistEle = $D.get("divplaylist"),
        _FmList = $D.get("single_radio_list"),
        _FmTitle = $D.get("FM_songname"),
        _FmSongInfo = $D.get("single_radio_info"),
        _clearList = $D.get("clear_list"),
        _divplayframe = $D.get("divplayframe"),
        _modeBtn = $D.get("btnPlayway"),
        _songNum = $D.get("spansongnum1"),
        _singOnPlayBegin = function(status) {
            if (status == "fm") g_webPlayer.OnSongPlayBegin = function(songinfo, index, total) {
                _updateMode(3);
                _playerOp._isPlaying = true;
                _playerOp._curSongInfo = songinfo;
                var _curFmSong = $D.getElementsByClassName("current", "li", _FmList)[0],
                _prevEle = $D.getPreviousSibling(_curFmSong),
                _nextEle = $D.getNextSibling(_curFmSong);
                if (!singleFm._delBtnStatus) if (singleFm.leaving && _curFmSong.getAttribute("mid") == songinfo.mid) singleFm.play();
                else if (_nextEle && _nextEle.getAttribute("mid") == songinfo.mid) singleFm.next();
                else {
                    if (!$C.hasClassName(_prevEle, "first")) singleFm.prev()
                } else singleFm._delBtnStatus = false;
                if (_nextEle && ($D.getNextSibling(_nextEle) && !$D.getNextSibling($D.getNextSibling(_nextEle)))) _model.nextPage();
                var uin = g_user.getUin();
                if (uin < 10001) {
                    _playerOp.renderSong(songinfo);
                    _renderFmList(songinfo)
                } else g_mymusicAlbumMod.Song.getMyfavMap(uin,
                function(mapMyfav) {
                    _playerOp.renderSong(songinfo, mapMyfav);
                    _renderFmList(songinfo, mapMyfav)
                });
                singleFm.status = true;
                singleFm.leaving = false;
                if (songinfo.mtype != "net") $D.get("qrc_ctn").innerHTML = "<p>&nbsp;</p><p>\u6b4c\u8bcd\u6b63\u5728\u52a0\u8f7d\u4e2d...</p>";
                else $D.get("qrc_ctn").innerHTML = '<p>&nbsp;</p><p><a href="http://support.qq.com/write.shtml?fid=602" target="_blank">\u8be5\u5355\u66f2\u6682\u65e0\u6b4c\u8bcd\uff0c\u70b9\u51fb\u8fd9\u91cc\u8fdb\u884c\u53cd\u9988</a></p>';
                g_qrcHandlerChn.clearQrcInfo();
                common.isQusicSong(_playerOp._curSongInfo) && g_qrcHandlerChn.init({
                    songMId: songinfo.mmid,
                    songId: songinfo.mid,
                    qrcContainer: "qrc_ctn"
                });
                g_lrcHandlerChn.clearLrcInfo();
                _volumeMod.init();
                _playProgressBar.init();
                var _ePlayer = $D.get("divplayer"),
                _target = $D.get("btnfold");
                if (_target.title == "\u70b9\u51fb\u5c55\u5f00") $C.addClassName(_ePlayer, "m_player_playing");
                setTimeout(function() {
                    _playerOp._setQQstatus(false, songinfo.mid)
                },
                700)
            };
            else g_webPlayer.OnSongPlayBegin = function(songinfo, index, total) {
                _playerOp._isPlaying = true;
                _playerOp._curSongInfo = songinfo;
                var uin = g_user.getUin();
                if (uin < 10001) _playerOp.renderSong(songinfo);
                else g_mymusicAlbumMod.Song.getMyfavMap(uin,
                function(mapMyfav) {
                    _playerOp.renderSong(songinfo, mapMyfav)
                });
                if (songinfo.mtype != "net") $D.get("qrc_ctn").innerHTML = "<p>&nbsp;</p><p>\u6b4c\u8bcd\u6b63\u5728\u52a0\u8f7d\u4e2d...</p>";
                else $D.get("qrc_ctn").innerHTML = '<p>&nbsp;</p><p><a href="http://support.qq.com/write.shtml?fid=602" target="_blank">\u8be5\u5355\u66f2\u6682\u65e0\u6b4c\u8bcd\uff0c\u70b9\u51fb\u8fd9\u91cc\u8fdb\u884c\u53cd\u9988</a></p>';
                g_qrcHandlerChn.clearQrcInfo();
                common.isQusicSong(_playerOp._curSongInfo) && g_qrcHandlerChn.init({
                    songMId: songinfo.mmid,
                    songId: songinfo.mid,
                    qrcContainer: "qrc_ctn"
                });
                g_lrcHandlerChn.clearLrcInfo();
                _volumeMod.init();
                _playProgressBar.init();
                var _ePlayer = $D.get("divplayer"),
                _target = $D.get("btnfold");
                if (_target.title == "\u70b9\u51fb\u5c55\u5f00") $C.addClassName(_ePlayer, "m_player_playing");
                setTimeout(function() {
                    var _e = $D.get("divsonglist");
                    if ( !! _e) {
                        var _es = _e.getElementsByTagName("li");
                        if (_es.length > index) {
                            if (_curPlayElem) {
                                $C.removeClassName(_curPlayElem, "play_current");
                                _curPlayElem = null
                            }
                            _curPlayElem = _es[index];
                            $C.removeClassName(_curPlayElem, "play_wrong");
                            $C.addClassName(_curPlayElem, "play_current");
                            _updateListScroll()
                        }
                    }
                },
                500);
                if ( !! g_msPinnedSite) {
                    g_msPinnedSite.paramsConfig.isPlaying = true;
                    g_msPinnedSite.paramsConfig.isPaused = false;
                    g_msPinnedSite.trogglePlayPauseBtn()
                }
                setTimeout(function() {
                    _playerOp._setQQstatus(false, songinfo.mid)
                },
                700)
            }
        },
        _viewTpl = function(songlist) {
            var _cache = "",
            _malbumId, _imgCache;
            for (var i = 0,
            len = songlist.length; i < len; i++) {
                _malbumId = songlist[i].malbummid,
                _imgCache = _malbumId.slice(_malbumId.length - 2, _malbumId.length - 1) + "/" + _malbumId.slice(_malbumId.length - 1, _malbumId.length) + "/" + _malbumId;
                _cache += '<li mid="' + songlist[i].mid + '"><a href="javascript:;"><img onerror="this.src=\'http://imgcache.qq.com/mediastyle/y/img/cover_mine_300.jpg\'" src="http://imgcache.qq.com/music/photo/mid_album_300/' + _imgCache + '.jpg"><i class="icon_radio_play"></i></a>"'
            }
            return _cache
        },
        _setFmList = function(dirid, callback) {
            g_webPlayer.setPlayerList(false, singleFm.curFMList, 2);
            _curPlayDirid = dirid;
            callback && callback();
            return
        },
        _model = function() {
            var cacheData = {},
            curSongData = null,
            curSid = null,
            end = false,
            next = 0,
            retimeStatus = false,
            reportCache = {
                MsgID: "",
                SceneID: "",
                PolicyID: ""
            },
            renewConfig = function(next, sid, end) {
                next = next;
                curSid = sid;
                end = end
            },
            getData = function(sid, cb, nextData) {
                sid = parseInt(sid);
                if (typeof sid != "number") return;
                cb = cb ||
                function() {};
                curSid = sid;
                if (cacheData[sid] && cacheData[sid][next]) {
                    renewConfig(next, sid, false);
                    cb(cacheData[sid][next]);
                    return cacheData[sid][next]
                }
                var url = "http://s.plcloud.music.qq.com/fcgi-bin/fcg_get_similar_song.fcg?id=" + sid + "&index=" + next + "&num=10&jsonpCallback=MusicJsonFmCallback&_=" + (new Date).valueOf();
                var j = new MUSIC.JSONGetter(url, "singleFM" + sid + next, null, "utf-8", false);
                j.onSuccess = function(data) {
                    if (data.code != 0) { ! nextData && Events.trigger("FMempty", _model, "\u60a8\u8bf7\u6c42\u7684\u6570\u636e\u51fa\u9519\u4e86\uff01");
                        return
                    }
                    var _FMdata = data.data;
                    if (!_FMdata.retime) {
                        retimeStatus = false;
                        if (_FMdata.total == 0) { ! nextData && Events.trigger("FMempty");
                            renewConfig(0, sid, true);
                            return
                        }
                        if (!cacheData[sid]) cacheData[sid] = {};
                        cacheData[sid][next] = _songlist;
                        if (_FMdata.next == -1 || _FMdata.next == 0) {
                            end = true;
                            next = 0
                        } else next = _FMdata.next;
                        renewConfig(next, sid, false)
                    } else retimeStatus = true;
                    if (_FMdata.call_back) {
                        var _cache = _FMdata.call_back.split("|"),
                        _obj;
                        for (var i = _cache.length - 1; i >= 0; i--) {
                            _obj = _cache[i].split(":");
                            if (_obj[0]) reportCache[_obj[0]] = _obj[1]
                        }
                    } else reportCache = {
                        MsgID: "",
                        SceneID: "",
                        PolicyID: ""
                    };
                    var songsData = _FMdata.similars;
                    if (songsData.length == 0) { ! nextData && Events.trigger("FMempty", _model, "\u60a8\u8bf7\u6c42\u7684\u6570\u636e\u5217\u8868\u4e3a\u7a7a\uff01");
                        renewConfig(0, sid, true);
                        return
                    }
                    var _songlist = [];
                    MUSIC.object.each(songsData,
                    function(music) {
                        var _cache = g_trackServ.formatMusic(music);
                        _cache.msinger && (_cache.msinger = _cache.msinger.unescapeHTML().unescapeHTML());
                        _cache.msong && (_cache.msong = _cache.msong.unescapeHTML().unescapeHTML());
                        _songlist.push(_cache)
                    });
                    cb(_songlist)
                };
                j.onError = function(err) {
                    singleFm.loadingData = false; ! nextData && Events.trigger("FMempty", _model, "\u60a8\u8bf7\u6c42\u7684\u6570\u636e\u5217\u8868\u4e3a\u7a7a\uff01");
                    renewConfig(1, sid, true);
                    throw err;
                };
                j.send("MusicJsonFmCallback")
            },
            nextPage = function() {
                if (end && !retimeStatus) return;
                singleFm.loadingData = true;
                getData(curSid,
                function(list) {
                    addPlayList(list)
                },
                true)
            },
            addPlayList = function(list) {
                _model.playSongData = list;
                Events.trigger("FmAddSong", _model, list)
            },
            changePlayModel = function(list) {
                var list = list.concat(singleFm.songInfo);
                _model.playSongData = list;
                singleFm.curFMList = list;
                Events.trigger("FmChange", _model, list)
            };
            return {
                playSongData: [],
                changePlayModel: changePlayModel,
                nextPage: nextPage,
                changeFMId: function(sid) {
                    sid = parseInt(sid);
                    if (typeof sid != "number") return;
                    if (this.data && this.data[sid]) {
                        changePlayModel(this.data[sid]);
                        return
                    }
                    getData(sid, changePlayModel)
                },
                report: function(type, data) {
                    var _callback = "";
                    for (var i in reportCache) _callback += "&" + i + "=" + reportCache[i];
                    var url = "http://s.plcloud.music.qq.com/fcgi-bin/fcg_similar_report.fcg?type=" + type + "&qq=" + data.qq + "&songid=" + data.songid + "&oper=" + data.oper + "&rid=" + data.rid + "&sid=" + data.sid + "&time=" + (data.time || 0) + _callback + "&g_tk=" + getACSRFToken() + "&playtime=" + (data.playtime || 0) + "&random=" + (new Date).valueOf();
                    if (!$D.get("reportImg")) {
                        var _img = document.createElement("img");
                        _img.setAttribute("id", "reportImg");
                        _img.setAttribute("src", url);
                        document.body.appendChild(_img)
                    } else $D.get("reportImg").setAttribute("src", url)
                }
            }
        } (),
        _toggleTab = function() {
            if (_fmTab.className.indexOf("current") >= 0) _cutTab("playlist");
            else _cutTab("FM");
            _updateListScroll()
        },
        _cutTab = function(tabName) {
            tabName = tabName || "FM";
            if (tabName == "FM") {
                _updateMode(3);
                _fmTab.style.display = "block";
                _fmTab.className = "current";
                _playListTab.className = "";
                _showFm();
                common.hideEle(_clearList);
                common.hideEle(_modeBtn);
                common.hideEle($D.get("divselect"));
                _songNum.className = "open_list radio_list_button"
            } else {
                _hideFm();
                _playListTab.className = "current";
                _fmTab.className = "";
                common.showEle(_modeBtn);
                common.showEle(_clearList);
                _updateListScroll();
                _updateSongNum();
                _songNum.className = "open_list"
            }
        },
        _showEmpty = function() {
            singleFm.status = false;
            common.showEle(_emptyList);
            common.hideEle(_FMEle);
            _emptyStatus = true;
            _updateListScroll()
        },
        _showFm = function() {
            if (_emptyStatus) _showEmpty();
            else common.showEle(_FMEle);
            common.hideEle(_divnulllist);
            common.hideEle(_playlistEle)
        },
        _hideFm = function() {
            common.hideEle(_FMEle);
            common.hideEle(_emptyList);
            if (_playListEmpty) {
                common.showEle(_divnulllist);
                common.hideEle(_playlistEle)
            } else {
                common.hideEle(_divnulllist);
                common.showEle(_playlistEle)
            }
        },
        _setFm = function(songInfo) {
            if (!songInfo) return;
            singleFm.songInfo = songInfo;
            singleFm.rid = songInfo.mid;
            singleFm.dirid = "FM" + songInfo.mid;
            singleFm.status = true;
            singleFm.firstCur = false
        },
        _resetFm = function() {
            singleFm.songInfo = null;
            singleFm.curFMList = null;
            singleFm.rid = null;
            singleFm.dirid = 0;
            singleFm.status = false
        },
        _renderFmList = function(songInfo, mapMyfav) {
            var _classLike;
            if ( !! mapMyfav && mapMyfav[songInfo.mmid]) {
                songInfo.class_like = "single_radio_liked";
                songInfo.title_like = "\u53d6\u6d88\u559c\u6b22"
            } else {
                songInfo.class_like = "single_radio_like";
                songInfo.title_like = "\u559c\u6b22"
            }
            songInfo.msongCut = songInfo.msongCut || songinfo.msong;
            var _html = '<a href="javascript:;" class="' + songInfo.class_like + '" title="' + songInfo.title_like + '"></a>' + '<a href="javascript:;" class="single_radio_delete" title="\u5220\u9664"></a>' + '<h3 title="' + songInfo.msong.unescapeHTML() + '">' + songInfo.msongCut.unescapeHTML() + "</h3>" + '<p title="' + songInfo.malbum.unescapeHTML() + '">' + songInfo.malbum.unescapeHTML() + "</p>";
            _FmSongInfo.innerHTML = _html
        },
        _updateListWidth = function() {
            _FmList.style.width = (singleFm.curFMList.length + 1) * 310 + "px"
        },
        _render = function(model, list) {
            if (model.playSongData.length == 0) {
                _showEmpty();
                return
            }
            var _html, _cache, uin;
            _updateListWidth();
            common.hideEle(_emptyList);
            singleFm.leaving = false;
            singleFm.dirid = "FM" + singleFm.rid;
            _html = _viewTpl([singleFm.songInfo].concat(model.playSongData.concat().splice(0, model.playSongData.length - 1)));
            _FmList.innerHTML += _html;
            _setFmList(singleFm.dirid);
            _FmTitle.innerHTML = singleFm.songInfo.msong;
            _cache = _FmList.getElementsByTagName("li")[1];
            _cache.className = "current";
            $D.getNextSibling(_cache).className = "next";
            uin = g_user.getUin();
            if (uin < 10001) _renderFmList(singleFm.songInfo);
            else g_mymusicAlbumMod.Song.getMyfavMap(uin,
            function(mapMyfav) {
                _renderFmList(singleFm.songInfo, mapMyfav)
            });
            _FmList.style.marginLeft = $C.hasClassName($D.get("divplayer"), "mini_version") ? "-95px": "-145px";
            singleFm.firstCur = true;
            _singOnPlayBegin("fm")
        },
        _addSong = function(model, list) {
            var _prevList = singleFm.curFMList,
            _source = _prevList.slice(_prevList.length - 1)[0],
            _sourceRemind = _source.mid == singleFm.rid;
            if (_sourceRemind) {
                var _len = _prevList.length - 1;
                list.push(_prevList.splice(_len, 1)[0]);
                g_webPlayer.delSong(_len, 1)
            }
            singleFm.curFMList = singleFm.curFMList.concat(list);
            g_webPlayer.addSong(list, false);
            _FmList.style.width = parseInt(_FmList.style.width) + list.length * 310 + "px";
            var _html = _viewTpl(_sourceRemind ? list.splice(0, list.length - 1) : list);
            _FmList.innerHTML += _html;
            setTimeout(function() {
                singleFm.loadingData = false
            },
            500)
        },
        _delCurSong = function() {
            if (singleFm.loadingData) {
                showTips("\u8bf7\u7a0d\u5019", "\u7cfb\u7edf\u6b63\u5728\u52a0\u8f7d\u66f4\u591a\u6b4c\u66f2", 1, "width:170px");
                if (_timeout) {
                    clearTimeout(_timeout);
                    _timeout = null
                }
                _timeout = setTimeout(function() {
                    hideTips();
                    _timeout = null
                },
                2E3);
                return
            }
            if (singleFm.deling) {
                showTips("\u8bf7\u7a0d\u5019", "\u7cfb\u7edf\u6b63\u5728\u52aa\u529b\u5220\u9664", 1, "width:150px");
                if (_timeout) {
                    clearTimeout(_timeout);
                    _timeout = null
                }
                _timeout = setTimeout(function() {
                    hideTips();
                    _timeout = null
                },
                2E3);
                return
            }
            singleFm.report(3, {
                qq: g_user.getUin() || 0,
                songid: _playerOp._curSongInfo.mid,
                oper: 3,
                rid: singleFm.rid,
                sid: _playerOp._curSongInfo.mid,
                time: _playerOp._curSongInfo.minterval,
                playtime: _playerOp._curSongInfo.curSongTime
            });
            var _curSong = $D.getElementsByClassName("current", "li", _FmList)[0],
            _nextSong,
            _nextNextSong,
            _mid = _curSong.getAttribute("mid");
            _pos = g_webPlayer.getPostion();
            if (_pos == -1) {
                _pos = singleFm.curFMList.length - 1;
                singleFm.firstCur = false;
                g_webPlayer.delSong(_pos, 1);
                g_webPlayer.nextPlayer()
            } else g_webPlayer.delSong(_pos, 1);
            singleFm.curFMList.splice(_pos, 1);
            if (singleFm.curFMList == 0) {
                _playerOp.renderSong();
                _playerOp._curSongInfo = null;
                _playProgressBar.destroy();
                setTimeout(function() {
                    _playerOp.setPlayBtnStatus("pause", "g_first_play")
                },
                30);
                singleFm._delBtnStatus = false;
                singleFm.close()
            } else $D.get("spanprogress_op").style.left = "0%";
            _nextSong = $D.getNextSibling(_curSong);
            if (!_nextSong) {
                var _prevSong = $D.getPreviousSibling(_curSong);
                g_webPlayer.lastPlayer();
                $D.removeElement(_curSong);
                return
            }
            _nextNextSong = $D.getNextSibling(_nextSong);
            _nextSong.className = "current";
            _nextNextSong && (_nextNextSong.className = "next");
            _curSong.innerHTML = "";
            _curSong.setAttribute("id", "js_del");
            singleFm.deling = true;
            if (!MUSIC.userAgent.webkit) MUSIC.effect.run(_curSong, {
                margin: "0",
                width: "0"
            },
            {
                duration: 800,
                complete: function() {
                    setTimeout(function() {
                        var _delSong = $D.get("js_del");
                        singleFm.deling = false;
                        _delSong.className = "";
                        $D.removeElement(_delSong)
                    },
                    800)
                }
            });
            else {
                _curSong.style.cssText = "margin:0;width:0;opacity:0;";
                setTimeout(function() {
                    var _delSong = $D.get("js_del");
                    singleFm.deling = false;
                    _delSong.className = "";
                    $D.removeElement(_delSong)
                },
                800)
            }
            $D.get("qrc_ctn").innerHTML = "";
            if (g_qrcHandlerChn.haveqrc) g_qrcHandlerChn.clearQrcInfo();
            else if (g_lrcHandlerChn.haveLrc) g_lrcHandlerChn.clearLrcInfo()
        },
        fmTips = {
            _closed: false,
            _tips: $D.get("single_radio_tip"),
            init: function() {},
            close: function() {}
        };
        return {
            init: function() {
                Events.on("FmChange", _render);
                Events.on("FMempty", _showEmpty);
                Events.on("FmAddSong", _addSong);
                Events.on("likeSong", singleFm.updateSongLike);
                fmTips.init();
                $E.addEvent(_tabContainer, "click",
                function(e) {
                    var _target = MUSIC.event.getTarget(e),
                    _regexp = eval("/^li$/i");
                    while (!_regexp.test(_target.tagName)) _target = _target.parentNode;
                    if ($C.hasClassName(_target, "current")) return;
                    _toggleTab()
                });
                $E.addEvent(_FmList, "click",
                function(e) {
                    if (singleFm.deling || (singleFm._nexting || singleFm._preving)) return;
                    var _target = MUSIC.event.getTarget(e),
                    _regexp = /^li$/ig;
                    if (/^ul$/ig.test(_target.tagName) || $C.hasClassName(_target, "first")) return;
                    var _dis = 0;
                    var _pos;
                    while (!_regexp.test(_target.tagName)) _target = _target.parentNode;
                    if (singleFm.leaving) {
                        _singOnPlayBegin("fm");
                        _setFmList(singleFm.dirid);
                        _pos = function() {
                            var _mid = _target.getAttribute("mid");
                            for (var i = 0,
                            len = singleFm.curFMList.length; i < len; i++) if (_mid == singleFm.curFMList[i].mid) return i;
                            return null
                        } ();
                        g_webPlayer.playAnyPos(_pos);
                        return
                    }
                    if (!$C.hasClassName(_target, "current")) {
                        if ($C.hasClassName(_target, "next")) g_webPlayer.nextPlayer();
                        else g_webPlayer.lastPlayer();
                        singleFm.report(2, {
                            qq: g_user.getUin() || 0,
                            songid: _playerOp._curSongInfo.mid,
                            oper: 7,
                            rid: singleFm.rid,
                            sid: _playerOp._curSongInfo.mid,
                            time: _playerOp._curSongInfo.minterval,
                            playtime: _playerOp._curSongInfo.curSongTime
                        })
                    } else if ($C.hasClassName(_target.getElementsByTagName("i")[0], "icon_radio_pause")) g_webPlayer.pausePlayer();
                    else g_webPlayer.startPlayer()
                });
                $E.addEvent(_FmSongInfo, "click",
                function(e) {
                    $E.cancelBubble();
                    var _target = MUSIC.event.getTarget(e);
                    if ($C.hasClassName(_target, "single_radio_like")) _playerOp.like(_playerOp._curSongInfo, _target);
                    else if ($C.hasClassName(_target, "single_radio_liked")) _playerOp.like(_playerOp._curSongInfo, _target);
                    if ($C.hasClassName(_target, "single_radio_delete")) {
                        singleFm._delBtnStatus = true;
                        _delCurSong();
                        singleFm._delBtnStatus = false
                    }
                })
            },
            next: function() {
                var _dis = $C.hasClassName($D.get("divplayer"), "mini_version") ? -220 : -270,
                _left,
                _target = $D.getElementsByClassName("next", "li", _FmList)[0],
                _tarNextEle,
                _tarPreEle,
                _delEle,
                _pos = 0;
                _tarPreEle = $D.getPreviousSibling(_target);
                _tarNextEle = $D.getNextSibling(_target);
                _delEle = $D.getPreviousSibling(_tarPreEle);
                if (!$C.hasClassName(_delEle, "first")) {
                    _delEle.innerHTML = "";
                    _delEle.className = "first";
                    if (singleFm.firstCur) {
                        _pos = singleFm.curFMList.length - 1;
                        singleFm.firstCur = false
                    }
                    singleFm.curFMList.splice(_pos, 1);
                    g_webPlayer.delSong(_pos, 1)
                }
                _tarPreEle.className = "prev";
                _tarPreEle.getElementsByTagName("i")[0].className = "icon_radio_play";
                _target.className = "current";
                _tarNextEle && (_tarNextEle.className = "next");
                singleFm._nexting = true;
                if (_dis) if (!MUSIC.userAgent.webkit) MUSIC.effect.run(_FmList, {
                    marginLeft: parseInt(_FmList.style.marginLeft) + _dis + "px"
                },
                {
                    duration: 800,
                    complete: function() {
                        singleFm._nexting = false
                    }
                });
                else {
                    _left = parseInt(_FmList.style.marginLeft) + _dis;
                    _FmList.style.marginLeft = _left + "px";
                    setTimeout(function() {
                        singleFm._nexting = false
                    },
                    800)
                }
                g_webPlayer.startPlayer()
            },
            prev: function() {
                var _dis = $C.hasClassName($D.get("divplayer"), "mini_version") ? 220 : 270,
                _left = parseInt(_FmList.style.marginLeft) + _dis,
                _target = $D.getElementsByClassName("prev", "li", _FmList)[0],
                _tarPreEle = $D.getPreviousSibling(_target),
                _tarNextEle = $D.getNextSibling(_target);
                singleFm._preving = true;
                if (!MUSIC.userAgent.webkit) MUSIC.effect.run(_FmList, {
                    marginLeft: _left + "px"
                },
                {
                    duration: 800,
                    complete: function() {
                        singleFm._preving = false
                    }
                });
                else {
                    _FmList.style.marginLeft = _left + "px";
                    singleFm._preving = false
                }
                if (!$C.hasClassName(_tarPreEle, "first")) _tarPreEle.className = "prev";
                _target.className = "current";
                if (_tarNextEle) {
                    var _delNextClassEle;
                    _tarNextEle.className = "next";
                    _tarNextEle.getElementsByTagName("i")[0].className = "icon_radio_play";
                    if (_delNextClassEle = $D.getNextSibling(_tarNextEle)) _delNextClassEle.className = ""
                }
                g_webPlayer.startPlayer()
            },
            updateSongLike: function(mid, status) {
                var _curSong = $D.getElementsByClassName("current", "li", _FmList)[0];
                if (_curSong && _curSong.getAttribute("mid") == mid) if (status) {
                    _FmSongInfo.getElementsByTagName("a")[0].className = "single_radio_liked";
                    singleFm.report(4, {
                        qq: g_user.getUin() || 0,
                        songid: _playerOp._curSongInfo.mid,
                        oper: 1,
                        rid: singleFm.rid,
                        sid: mid,
                        time: _playerOp._curSongInfo.minterval,
                        playtime: _playerOp._curSongInfo.curSongTime
                    })
                } else {
                    singleFm.report(4, {
                        qq: g_user.getUin() || 0,
                        songid: _playerOp._curSongInfo.mid,
                        oper: 2,
                        rid: singleFm.rid,
                        sid: mid,
                        time: _playerOp._curSongInfo.minterval,
                        playtime: _playerOp._curSongInfo.curSongTime
                    });
                    _FmSongInfo.getElementsByTagName("a")[0].className = "single_radio_like"
                }
            },
            open: function() {
                var _songInfo = _playerOp._curSongInfo;
                $E.cancelBubble();
                if (_songInfo) {
                    if (singleFm.rid == _songInfo.mid) {
                        if (_divplayframe.style.display == "block") hidePlaylist();
                        else {
                            showPlaylist();
                            _cutTab("FM")
                        }
                        return
                    }
                    _emptyStatus = false;
                    fmTips.close();
                    common.hideEle($D.get("addSongTips"));
                    if (_curPlayElem) {
                        $C.removeClassName(_curPlayElem, "play_current");
                        _curPlayElem = null
                    }
                    _stopTime(); ! (_divplayframe.style.display == "block") && showPlaylist();
                    _setFm(_songInfo);
                    _cutTab("FM");
                    _showFm();
                    _FmList.innerHTML = '<li class="first"></li>';
                    _model.changeFMId(singleFm.rid);
                    singleFm.report("new", {
                        rid: singleFm.rid,
                        qq: g_user.getUin() || 0,
                        songid: singleFm.rid,
                        oper: 9,
                        time: 0,
                        sid: singleFm.rid
                    })
                } else {
                    showTips("\u8bf7\u5148\u64ad\u653e\u4e00\u9996\u6b4c", "\u968f\u4fbf\u64ad\u4e00\u9996\u8bd5\u8bd5", 1, "width:180px");
                    _timeout = setTimeout(function() {
                        hideTips();
                        _timeout = null
                    },
                    4E3)
                }
            },
            close: function() {
                _singOnPlayBegin("playlist");
                _cutTab("playlist");
                _resetFm();
                common.hideEle(_fmTab)
            },
            play: function() {
                if (singleFm.status) {
                    var _curSong = $D.getElementsByClassName("current", "li", _FmList)[0];
                    if (_curSong) {
                        _curSong = _curSong.getElementsByTagName("i")[0];
                        _curSong && (_curSong.className = "icon_radio_pause")
                    }
                }
            },
            pause: function() {
                if (singleFm.status) {
                    var _curSong = $D.getElementsByClassName("current", "li", _FmList)[0];
                    if (_curSong) {
                        _curSong = _curSong.getElementsByTagName("i")[0];
                        _curSong && (_curSong.className = "icon_radio_play")
                    }
                }
            },
            report: function(type, data) {
                type = type || 0;
                data = data || {};
                _model.report(type, data)
            },
            singOnPlayBegin: _singOnPlayBegin,
            leaving: false,
            loadingData: false,
            cutTab: _cutTab,
            deling: false,
            _preving: false,
            _nexting: false,
            _timeout: null,
            _delBtnStatus: false,
            songInfo: null,
            status: false,
            firstCur: false
        }
    } ();
    function init() {
        if (window.screen.height <= 768) $C.addClassName($D.get("divplayer"), "mini_version");
        g_webPlayer.init({
            fromtag: 30,
            statFromtag: 7,
            errorTips: function(title, text) {
                g_popup.show(1, title, text, 3E3, 200)
            }
        });
        _localSong.get(function(list) {
            $D.get("spansongnum1").onclick = function() {
                _stopTime();
                if ($D.get("divplayframe").style.display == "none") showPlaylist();
                else hidePlaylist()
            };
            _updateSongNum();
            $E.addEvent($D.get("btnclose"), "click", hidePlaylist);
            $E.addEvent($D.get("btnfold"), "click", trogglePlayer);
            $E.addEvent($D.get("btnlrc"), "click", troggleLrcPannel);
            $E.addEvent($D.get("closelrcpannel"), "click", troggleLrcPannel);
            listFrame.init(list);
            _playerOp.init();
            var _timer = null;
            function _startScroll() {
                if (_timer) clearTimeout(_timer);
                else if (ua.ie == 6) hideElement("divplayer");
                else;
                hideElement("divyellowtips");
                _timer = setTimeout(_stopScroll, 500)
            }
            function _stopScroll() {
                if (_timer) {
                    clearTimeout(_timer);
                    _timer = null;
                    if (ua.ie == 6) {
                        var _e = $D.get("divplayer");
                        _e.style.bottom = "";
                        _e.style.bottom = "10px";
                        showElement(_e)
                    } else;
                }
            }
            if (ua.ie == 6) $E.addEvent(window, "scroll", _startScroll);
            $E.addEvent($D.get("divsongframe"), "click", _stopTime);
            $E.addEvent($D.get("divsonglist"), "dblclick",
            function() {
                window.getSelection ? window.getSelection().removeAllRanges() : document.selection.empty()
            });
            setTimeout(function() {
                trogglePlayer();
                _startTime()
            },
            500)
        });
        try {
            var _playlist = $D.get("divlistmain");
            $E.addEvent(_playlist, "dragenter",
            function(evt) {
                var _e = $E.getTarget(evt).parentNode;
                if (_e.nodeName === "LI" && ( !! top["drag_songinfo"] && top["drag_songinfo"].trim() != "")) $C.addClassName(_e, "move_here")
            });
            $E.addEvent(_playlist, "dragover",
            function(evt) {
                $E.preventDefault()
            });
            $E.addEvent(_playlist, "dragleave",
            function(evt) {
                var _e = $E.getTarget(evt).parentNode;
                if (_e.nodeName === "LI") $C.removeClassName(_e, "move_here")
            });
            $E.addEvent(_playlist, "drop",
            function(evt) {
                if ( !! top["drag_songinfo"] && top["drag_songinfo"].trim() != "") {
                    var _obj = g_trackServ.formatMusic(top["drag_songinfo"]);
                    var _e = $E.getTarget(evt).parentNode;
                    if (_e.nodeName === "LI") {
                        $C.removeClassName(_e, "move_here");
                        top.g_topPlayer.insert(_e, _obj)
                    } else top.g_topPlayer.add([_obj], false)
                }
            });
            $E.addEvent($D.get("btnfold"), "dragenter",
            function(evt) {
                if ( !! top["drag_songinfo"] && top["drag_songinfo"].trim() != "") {
                    var _e = $E.getTarget(evt);
                    if (_e.title == "\u70b9\u51fb\u5c55\u5f00") trogglePlayer()
                }
            });
            $E.addEvent($D.get("divplayer"), "dragenter",
            function(evt) {
                if ( !! top["drag_songinfo"] && top["drag_songinfo"].trim() != "") {
                    _stopTime();
                    if ($D.get("divplayframe").style.display == "none") showPlaylist()
                }
            });
            $E.addEvent($D.get("divplayer"), "dragover",
            function(evt) {
                $E.preventDefault()
            });
            $E.addEvent($D.get("divplayer"), "drop",
            function(evt) {
                if ( !! top["drag_songinfo"] && top["drag_songinfo"].trim() != "") {
                    var _obj = g_trackServ.formatMusic(top["drag_songinfo"]);
                    top.g_topPlayer.add([_obj], false)
                }
            })
        } catch(e) {}
        singleFm.init();
        var _iconRadio = $D.get("icon_radio");
        _iconRadio && _iconRadio.addEventListener("click", singleFm.open)
    }
    return {
        init: init,
        showPlaylist: showPlaylist,
        hidePlaylist: hidePlaylist,
        trogglePlayer: trogglePlayer,
        showTips: showTips,
        hideTips: hideTips,
        songHover: _playerOp.songHover,
        play: _playerOp.play,
        trogglePlay: _playerOp.trogglePlay,
        prev: _playerOp.prev,
        next: _playerOp.next,
        add: _playerOp.add,
        share: _playerOp.share,
        like: _playerOp.like,
        fav: _playerOp.fav,
        del: _playerOp.del,
        insert: _playerOp.insert,
        clearList: _playerOp.clearList,
        setPlayWay: _playerOp.setPlayWay,
        realSetPlayWay: _playerOp.realSetPlayWay,
        resetLikeIcon: _playerOp.resetLikeIcon,
        updateSongLike: _playerOp.updateSongLike,
        getCurSongInfo: _playerOp.getCurSongInfo,
        showLrcTips: showLrcTips,
        hideLrcTips: hideLrcTips,
        troggleLrcPannel: troggleLrcPannel,
        incVol: _volumeMod.incVol,
        decVol: _volumeMod.decVol,
        mute: _volumeMod.mute,
        showDragTips: showDragTips,
        hideDragTips: hideDragTips,
        singleFm: singleFm
    }
} ();
var g_topPlayer = MUSIC.channel.top_player;
MUSIC.module.coder = function() {
    var _this = this;
    var Base64 = {
        _keyStr: "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=",
        encode: function(input) {
            var output = "";
            var chr1, chr2, chr3, enc1, enc2, enc3, enc4;
            var i = 0;
            input = Base64._utf8_encode(input);
            while (i < input.length) {
                chr1 = input.charCodeAt(i++);
                chr2 = input.charCodeAt(i++);
                chr3 = input.charCodeAt(i++);
                enc1 = chr1 >> 2;
                enc2 = (chr1 & 3) << 4 | chr2 >> 4;
                enc3 = (chr2 & 15) << 2 | chr3 >> 6;
                enc4 = chr3 & 63;
                if (isNaN(chr2)) enc3 = enc4 = 64;
                else if (isNaN(chr3)) enc4 = 64;
                output = output + Base64._keyStr.charAt(enc1) + Base64._keyStr.charAt(enc2) + Base64._keyStr.charAt(enc3) + Base64._keyStr.charAt(enc4)
            }
            return output
        },
        decode: function(input) {
            var output = "";
            var chr1, chr2, chr3;
            var enc1, enc2, enc3, enc4;
            var i = 0;
            input = input.replace(/[^A-Za-z0-9\+\/\=]/g, "");
            while (i < input.length) {
                enc1 = Base64._keyStr.indexOf(input.charAt(i++));
                enc2 = Base64._keyStr.indexOf(input.charAt(i++));
                enc3 = Base64._keyStr.indexOf(input.charAt(i++));
                enc4 = Base64._keyStr.indexOf(input.charAt(i++));
                chr1 = enc1 << 2 | enc2 >> 4;
                chr2 = (enc2 & 15) << 4 | enc3 >> 2;
                chr3 = (enc3 & 3) << 6 | enc4;
                output = output + String.fromCharCode(chr1);
                if (enc3 != 64) output = output + String.fromCharCode(chr2);
                if (enc4 != 64) output = output + String.fromCharCode(chr3)
            }
            output = Base64._utf8_decode(output);
            return output
        },
        _utf8_encode: function(string) {
            string = string.replace(/\r\n/g, "\n");
            var utftext = "";
            for (var n = 0; n < string.length; n++) {
                var c = string.charCodeAt(n);
                if (c < 128) utftext += String.fromCharCode(c);
                else if (c > 127 && c < 2048) {
                    utftext += String.fromCharCode(c >> 6 | 192);
                    utftext += String.fromCharCode(c & 63 | 128)
                } else {
                    utftext += String.fromCharCode(c >> 12 | 224);
                    utftext += String.fromCharCode(c >> 6 & 63 | 128);
                    utftext += String.fromCharCode(c & 63 | 128)
                }
            }
            return utftext
        },
        _utf8_decode: function(utftext) {
            var string = "";
            var i = 0;
            var c = c1 = c2 = 0;
            while (i < utftext.length) {
                c = utftext.charCodeAt(i);
                if (c < 128) {
                    string += String.fromCharCode(c);
                    i++
                } else if (c > 191 && c < 224) {
                    c2 = utftext.charCodeAt(i + 1);
                    string += String.fromCharCode((c & 31) << 6 | c2 & 63);
                    i += 2
                } else {
                    c2 = utftext.charCodeAt(i + 1);
                    c3 = utftext.charCodeAt(i + 2);
                    string += String.fromCharCode((c & 15) << 12 | (c2 & 63) << 6 | c3 & 63);
                    i += 3
                }
            }
            return string
        }
    };
    return {
        Base64: Base64
    }
} ();
var g_coder = MUSIC.module.coder;
MUSIC.channel.qrcHandler = function() {
    function parseTxt2XML(xmlText) {
        var xmlDoc;
        if (typeof xmlText == "undefined" || "" == xmlText) return;
        if (window.DOMParser) {
            parser = new DOMParser;
            xmlDoc = parser.parseFromString(xmlText, "text/xml")
        } else {
            xmlDoc = new ActiveXObject("Microsoft.XMLDOM");
            xmlDoc.async = "false";
            xmlDoc.loadXML(xmlText)
        }
        return xmlDoc
    }
    function getTime() {
        return g_webPlayer.getCurPostion()
    }
    return function() {
        var _this = this,
        $D = MUSIC.dom,
        $E = MUSIC.event;
        _this.playingSong = "0";
        _this.reqFlag = false;
        _this.haveqrc = false;
        _this.qrcData = "";
        _this.qrcJson = {
            ar: "",
            ti: "",
            al: "",
            by: "",
            offset: "",
            lines: []
        };
        _this.lineItem = {
            timeTag: {
                startTime: 0,
                timeLen: 0
            },
            context: "",
            wordsItem: []
        };
        _this.wordItem = {
            word: "",
            startTime: 0,
            timeLen: 0
        };
        _this.qrcLineIndex = 0;
        _this.artist = "";
        _this.songTitle = "";
        _this.album = "";
        _this.byBody = "";
        _this.offset = 0;
        _this.qrcContainer = "";
        _this.init = function(opt) {
            _this.playingSong = !opt.songMId || opt.songMId === "0" ? parseInt(opt.songId) : opt.songMId;
            _this.qrcContainer = opt.qrcContainer;
            _this.clearQrcInfo();
            _this.loadLyricsBySongId(opt.songMId, _this.dealLyricsSucc, _this.dealLyricsFail)
        };
        _this.loadLyricsBySongId = function(songMId, succCallback, errCallback) {
            if (!songMId || songMId == "0") songMId = _this.playingSong;
            if (typeof errCallback !== "Funciton") errCallback = _this.dealLyricsFail;
            try { ! top["qqmusic_lyrics_qrc" + songMId] && (top["qqmusic_lyrics_qrc" + songMId] = "")
            } catch(e) {}
            var _data = null,
            _lyric = null;
            try {
                _data = top["qqmusic_lyrics_qrc" + songMId];
                _lyric = top["qqmusic_lyrics_lrc" + songMId]
            } catch(e) {
                _data = null;
                _lyric = null
            }
            if ( !! _data) {
                _data = g_coder.Base64.decode(_data);
                _this.reqFlag = false;
                _this.haveqrc = true;
                succCallback(_data);
                return
            } else if ( !! _lyric) {
                errCallback();
                return
            }
            _this.reqFlag = true;
            var _url = "http://portalcgi.music.qq.com/fcgi-bin/fcg_query_lyric.fcg?pcachetime=" + (new Date).getTime(),
            _jMode = !!ua.safari || ( !! ua.chrome || ( !! ua.isiPad || ( !! ua.isiPhone || !!ua.firefox))) ? true: false;
            if (typeof songMId !== "string") _url += "&musicid=" + _this.playingSong;
            else _url += "&songmid=" + songMId;
            var xmlAjax = new MUSIC.JSONGetter(_url, "song_lyrics", null, "utf-8", _jMode);
            xmlAjax.timeout = 3E3;
            xmlAjax.onSuccess = function(data) {
                if ( !! data && data.retcode == 0) if (data.type == 3) {
                    _this.reqFlag = false;
                    _this.haveqrc = true;
                    _data = data.lyric;
                    try {
                        top["qqmusic_lyrics_qrc" + songMId] = _data
                    } catch(e) {}
                    _data = g_coder.Base64.decode(_data);
                    succCallback(_data)
                } else {
                    errCallback();
                    if ( !! data.lyric) try {
                        top["qqmusic_lyrics_lrc" + songMId] = data.lyric
                    } catch(e) {}
                } else if ( !! data && data.retcode == 1) {
                    errCallback();
                    try {
                        top["qqmusic_lyrics_lrc" + songMId] = "no-lyrics"
                    } catch(e) {}
                } else errCallback()
            };
            xmlAjax.onTimeout = function() {
                xmlAjax._waiting = false;
                errCallback()
            };
            xmlAjax.onError = errCallback;
            xmlAjax.send("MusicJsonCallback")
        };
        _this.dealLyricsSucc = function(xmlHttp) {
            var _xmlDom = parseTxt2XML(xmlHttp);
            if (_xmlDom == undefined || typeof _xmlDom == "undefined") {
                _this.dealLyricsFail();
                return
            }
            var _QrcInfo = _xmlDom.getElementsByTagName("LyricInfo");
            var _originQrc = "";
            if ( !! _QrcInfo[0]) _originQrc = _QrcInfo[0].getElementsByTagName("Lyric_1")[0];
            else {
                _this.dealLyricsFail();
                return
            }
            if ( !! _originQrc) {
                _this.qrcData = _originQrc.getAttribute("LyricContent");
                _this.parseQrcData()
            }
        };
        _this.dealLyricsFail = function() {
            _this.reqFlag = false;
            _this.haveqrc = false;
            try {
                top["qqmusic_lyrics_qrc" + _this.playingSong] = null
            } catch(e) {}
        };
        _this.parseQrcData = function() {
            var _qrcData = _this.qrcData,
            _tmpLineObj = {},
            i = 0,
            _len = 0,
            _cache = {};
            _tmpLineObj = _this.getLineFlag(_qrcData);
            var _reg = new RegExp("_!!_"),
            _tmpList = _tmpLineObj.rQrcStr.trim().split(_reg),
            _tmpLineTags = _tmpLineObj.lineFlags;
            for (i = 1, _len = _tmpList.length; i < _len; i++) {
                _cache = _tmpList[i];
                _reg = new RegExp("[\\[|\\]]", "g");
                var _tmpTag = _tmpLineTags[i - 1].replace(_reg, ""),
                _pointPos = 0;
                if (_tmpTag.indexOf("al:") != -1) {
                    _pointPos = _tmpTag.indexOf(":");
                    _this.album = _this.qrcJson.al = _tmpTag.substring(_pointPos + 1)
                } else if (_tmpTag.indexOf("ar:") != -1) {
                    _pointPos = _tmpTag.indexOf(":");
                    _this.artist = _this.qrcJson.ar = _tmpTag.substring(_pointPos + 1)
                } else if (_tmpTag.indexOf("ti:") != -1) {
                    _pointPos = _tmpTag.indexOf(":");
                    _this.songTitle = _this.qrcJson.ti = _tmpTag.substring(_pointPos + 1)
                } else if (_tmpTag.indexOf("by:") != -1) {
                    _pointPos = _tmpTag.indexOf(":");
                    _this.byBody = _this.qrcJson.by = _tmpTag.substring(_pointPos + 1)
                } else if (_tmpTag.indexOf("offset:") != -1) {
                    _pointPos = _tmpTag.indexOf(":");
                    _this.offset = _this.qrcJson.offset = _tmpTag.substring(_pointPos + 1)
                } else {
                    var _tmpTimes = _tmpTag.split(","),
                    _tmpObj = _this.getWordFlag(_cache.replace("``````````", ""));
                    _this.qrcJson.lines.push({
                        timeTag: {
                            startTime: parseInt(_tmpTimes[0].trim(), 10) - _this.offset,
                            timeLen: parseInt(_tmpTimes[1].trim(), 10)
                        },
                        context: !!_tmpObj ? _tmpObj.rLineStr: "",
                        wordsItem: !!_tmpObj ? _tmpObj.wordFlags: []
                    })
                }
            }
        };
        _this.printQrcLines = function() {
            var _json = _this.qrcJson;
            if ( !! _json) {
                var _lines = _json.lines,
                _len = _lines.length,
                i = 0,
                _html = [];
                for (; i < _len; i++) {
                    var _h5 = !!ua.safari || ( !! ua.chrome || ( !! ua.isiPad || !!ua.isiPhone));
                    if ( !! _h5) _html.push('<p id="line_' + i + '">' + _lines[i].context + "</p>");
                    else _html.push('<p id="line_' + i + '"><span>' + _lines[i].context + "</span></p>")
                }
                return _html.join("")
            }
            return ""
        };
        _this.getLineFlag = function(qrcStr) {
            qrcStr = qrcStr || _this.qrcData;
            var _tmpStr = qrcStr,
            reg = new RegExp("\\[[a-z|0-9]+\\s*[:|,]\\s*[^\\]]*\\]", "gi"),
            lineFlags = [],
            rtnObj = {},
            rQrcStr = "";
            lineFlags = _tmpStr.match(reg);
            rQrcStr = _tmpStr.replace(reg, "``````````_!!_");
            return {
                lineFlags: lineFlags,
                rQrcStr: rQrcStr
            }
        };
        _this.getWordFlag = function(lineTxt) {
            lineTxt = lineTxt.trim() || "";
            if ("" == lineTxt) return null;
            var _tmpLineTxt = lineTxt,
            reg = new RegExp("\\([-0-9]+,\\s*[-0-9]+\\)", "g"),
            wordFlags = [],
            _wordFlag = {},
            words = [],
            rtnObj = {},
            rtnWordFlags = [],
            rtnLineStr = "";
            wordFlags = _tmpLineTxt.match(reg);
            words = _tmpLineTxt.replace(reg, "_!!_").split("_!!_");
            rtnLineStr = _tmpLineTxt.replace(reg, "");
            if (wordFlags == null) {
                rtnObj = {
                    wordFlags: [],
                    rLineStr: rtnLineStr
                };
                return rtnObj
            }
            for (var i = 0,
            _len = wordFlags.length; i < _len; i++) {
                var _tmp = wordFlags[i],
                wordTimes = _tmp.replace(new RegExp("[\\(|\\)]", "g"), "").split(",");
                _wordFlag = {
                    word: words[i],
                    startTime: parseInt(wordTimes[0].trim(), 10) - _this.offset,
                    timeLen: parseInt(wordTimes[1].trim(), 10)
                };
                rtnWordFlags.push(_wordFlag)
            }
            rtnObj = {
                wordFlags: rtnWordFlags,
                rLineStr: rtnLineStr
            };
            return rtnObj
        };
        _this.clearQrcInfo = function() {
            _this.haveqrc = false;
            _this.qrcJson = {
                ar: "",
                ti: "",
                al: "",
                by: "",
                offset: "",
                lines: []
            };
            _this.qrcLineIndex = 0;
            _this.qrcData = "";
            _this.artist = "";
            _this.songTitle = "";
            _this.album = "";
            _this.byBody = "";
            _this.offset = 0;
            _this.reqFlag = false;
            _this.stopAnimFrame()
        };
        _this.clearQrcItem = function() {
            _this.lineItem = {
                timeTag: {
                    startTime: 0,
                    timeLen: 0
                },
                context: "",
                wordsItem: []
            };
            _this.wordItem = {
                word: "",
                startTime: 0,
                timeLen: 0
            }
        };
        _this.getQrcLineItemByPlayTime = function(playTime) {
            playTime = playTime || 0;
            var _curLineIndex = _this.qrcLineIndex,
            _curLineItem = _this.qrcJson.lines[_curLineIndex],
            i = 0,
            _len = _this.qrcJson.lines.length - 1,
            _line = {};
            _this.clearQrcItem();
            for (; i < _len; i++) {
                _line = _this.qrcJson.lines[i];
                var _start = _line.timeTag.startTime,
                _delay = _line.timeTag.timeLen,
                _dx = playTime - _start;
                if (_dx >= 0 && _dx <= _delay) {
                    _this.lineItem = _line;
                    _this.qrcLineIndex = i;
                    return true
                }
            }
            _line = _this.qrcJson.lines[_len];
            _start = _line.timeTag.startTime;
            _delay = _line.timeTag.timeLen;
            _dx = playTime - _start;
            if (_dx >= 0) {
                _this.lineItem = _line;
                _this.qrcLineIndex = _len;
                return true
            }
            return false
        };
        _this.getHighlightWords = function(playTime, lineIndex) {
            playTime = playTime || 0;
            lineIndex = lineIndex || _this.qrcLineIndex;
            if (playTime < _this.qrcJson.lines[0].timeTag.startTime) return "";
            var _line = _this.qrcJson.lines[lineIndex],
            _wordsItem = _line.wordsItem,
            _start = _line.timeTag.startTime,
            i = 0,
            _len = _wordsItem.length,
            _delay = 0,
            _word = {},
            _highlightWords = [];
            for (; i < _len; i++) {
                _word = _wordsItem[i];
                _delay += parseInt(_word.timeLen, 10);
                if (_delay > playTime - _start) {
                    _highlightWords.push(_word.word);
                    return _highlightWords.join("")
                }
                _highlightWords.push(_word.word)
            }
        };
        _this.scrollLine = function(elem) {
            elem = typeof elem == "string" ? $D.get(elem) : elem;
            var max = parseInt(elem.scrollHeight - 72),
            _top = elem.scrollTop,
            _line_height = !!$D.get("line_" + g_qrcHandlerChn.qrcLineIndex) ? $D.get("line_" + g_qrcHandlerChn.qrcLineIndex).offsetHeight: 24,
            stop = false,
            _idx = g_qrcHandlerChn.qrcLineIndex || 0,
            heights = 0,
            _times = parseInt(_line_height / 6, 10),
            _off = _line_height % 6;
            while (_idx-->0) {
                var _h = !!$D.get("line_" + _idx) ? $D.get("line_" + _idx).offsetHeight: 24;
                heights += _h
            }
            stop = _top == heights;
            if (!stop) if (heights - _top >= _line_height * 2) elem.scrollTop = heights;
            else {
                var _diff = heights - _top;
                if (_diff > _line_height) _diff -= _line_height;
                var _t = _diff >= _times ? _times: _off;
                elem.scrollTop = _top + _t
            }
            g_qrcHandlerChn.startAnimFrame(elem)
        };
        _this.aniFrame = null;
        _this.qrcAnimFrame = function(cb, delay) {
            return window.requestTimeout(cb, delay)
        };
        _this.startAnimFrame = function(elem) {
            g_qrcHandlerChn.aniFrame = g_qrcHandlerChn.qrcAnimFrame(function() {
                g_qrcHandlerChn.scrollLine(elem)
            },
            delay)
        };
        _this.stopAnimFrame = function() {
            if (g_qrcHandlerChn.aniFrame != null) window.cancelRequestAnimFrame(g_qrcHandlerChn.aniFrame);
            g_qrcHandlerChn.aniFrame = null
        };
        _this.restartAnimFrame = function(elem) {
            g_qrcHandlerChn.stopAnimFrame();
            g_qrcHandlerChn.startAnimFrame(elem)
        }
    }
} ();
var g_qrcHandlerChn = new MUSIC.channel.qrcHandler;
var delay = 1E3 / 60 * 12; (function() {
    window.requestAnimFrame = function() {
        return window.requestAnimationFrame || (window.webkitRequestAnimationFrame || (window.mozRequestAnimationFrame || (window.oRequestAnimationFrame || (window.msRequestAnimationFrame ||
        function(callback, element) {
            window.setTimeout(callback, 1E3 / 60)
        }))))
    } ();
    window.cancelRequestAnimFrame = function(handle) {
        window.cancelAnimationFrame ? window.cancelAnimationFrame(handle.value) : window.webkitCancelRequestAnimationFrame ? window.webkitCancelRequestAnimationFrame(handle.value) : window.mozCancelRequestAnimationFrame ? window.mozCancelRequestAnimationFrame(handle.value) : window.oCancelRequestAnimationFrame ? window.oCancelRequestAnimationFrame(handle.value) : window.msCancelRequestAnimationFrame ? window.msCancelRequestAnimationFrame(handle.value) : window.clearTimeout(handle)
    };
    window.requestTimeout = function(cb, delay) {
        if (!window.requestAnimationFrame && (!window.webkitRequestAnimationFrame && (!window.mozRequestAnimationFrame && (!window.oRequestAnimationFrame && !window.msRequestAnimationFrame)))) return window.setTimeout(cb, delay);
        var start = (new Date).getTime(),
        handle = new Object;
        function loop() {
            var current = (new Date).getTime(),
            delta = current - start;
            delta >= delay ? cb.call() : handle.value = window.requestAnimFrame(loop)
        }
        handle.value = window.requestAnimFrame(loop);
        return handle
    }
})();
MUSIC.channel.lrcHandler = function() {
    function sortFunc(obj1, obj2) {
        return g_lrcHandlerChn.compare(obj1.time, obj2.time) ? -1 : 1
    }
    function getTime() {
        return g_webPlayer.getCurPostion()
    }
    return function() {
        var _this = this,
        $D = MUSIC.dom,
        $E = MUSIC.event;
        _this.reqFlag = -1;
        _this.haveLrc = false;
        _this.lyricData = "";
        _this.lrcItem = {
            time: "00:00.00",
            context: ""
        };
        _this.lrcItemIndex = 0;
        _this.preLrcItemIndex = -1;
        _this.lrcList = [];
        _this.artist = "";
        _this.songTitle = "";
        _this.album = "";
        _this.byBody = "";
        _this.offset = 0;
        _this.playingSong = 0;
        _this.init = function(songMId) {
            _this.clearLrcInfo();
            _this.playingSong = songMId;
            _this.loadLyricBySongId(songMId, _this.dealLrcXmlSucc, _this.dealLrcXmlFail)
        };
        _this.loadLyricBySongId = function(songMId, succCallback, errCallback) {
            if (typeof errCallback !== "Funciton") errCallback = _this.dealLrcXmlFail;
            try { ! top["qqmusic_lyrics_lrc" + songMId] && (top["qqmusic_lyrics_lrc" + songMId] = null)
            } catch(e) {}
            var _data = null;
            try {
                _data = top["qqmusic_lyrics_lrc" + songMId]
            } catch(e) {
                _data = null
            }
            if ( !! _data) {
                _this.reqFlag = 1;
                _this.haveLrc = true;
                if (_data == "no-lyrics") {
                    errCallback();
                    return
                }
                _data = g_coder.Base64.decode(_data);
                succCallback(_data);
                return
            }
            _this.reqFlag = 0;
            var _url = "http://portalcgi.music.qq.com/fcgi-bin/fcg_query_lyric.fcg?pcachetime=" + (new Date).getTime(),
            _jMode = !!ua.safari || ( !! ua.chrome || ( !! ua.isiPad || ( !! ua.isiPhone || !!ua.firefox))) ? true: false;
            if (typeof songMId !== "string") _url += "&musicid=" + songMId;
            else _url += "&songmid=" + songMId;
            var xmlAjax = new MUSIC.JSONGetter(_url, "song_lyrics", null, "utf-8", _jMode);
            xmlAjax.timeout = 3E3;
            xmlAjax.onSuccess = function(data) {
                if ( !! data && data.retcode == 0) if (data.type != 3) {
                    _this.reqFlag = 1;
                    _this.haveLrc = true;
                    _data = data.lyric;
                    try {
                        top["qqmusic_lyrics_lrc" + songMId] = _data
                    } catch(e) {}
                    _data = g_coder.Base64.decode(_data);
                    succCallback(_data)
                } else errCallback();
                else errCallback()
            };
            xmlAjax.onTimeout = function() {
                xmlAjax._waiting = false;
                errCallback()
            };
            xmlAjax.onError = errCallback;
            xmlAjax.send("MusicJsonCallback")
        };
        _this.dealLrcXmlSucc = function(xmlHttp) {
            var _originLyric = xmlHttp;
            if ( !! _originLyric) {
                _this.lyricData = _originLyric.replace(new RegExp("\\n", "g"), "_!!_").trim();
                _this.parseLyricData()
            }
        };
        _this.dealLrcXmlFail = function() {
            g_lrcHandlerChn.reqFlag = 1;
            g_lrcHandlerChn.haveLrc = false;
            try {
                top["qqmusic_lyrics_lrc" + _this.playingSong] = null
            } catch(e) {}
        };
        _this.parseLyricData = function() {
            var _lrcData = _this.lyricData,
            _tmpList = [],
            i = 0,
            _len = 0,
            _cache;
            _tmpList = _lrcData.split("_!!_");
            for (_len = _tmpList.length; i < _len; i++) {
                _cache = _tmpList[i];
                var _rIndex = _cache.lastIndexOf("]"),
                _lSubstr = _cache.substring(0, _rIndex + 1),
                _rSubstr = _cache.substring(_rIndex + 1),
                _tmpTimes = _lSubstr.replace(new RegExp("\\[", "g"), "").split("]"),
                _tLen = _tmpTimes.length,
                j = _tLen - 1,
                _tmpTag = "",
                _pointPos = 0;
                while (j--) {
                    _tmpTag = _tmpTimes[j];
                    if (_tmpTag.indexOf("al:") != -1) {
                        _pointPos = _tmpTag.indexOf(":");
                        _this.album = _tmpTag.substring(_pointPos + 1)
                    } else if (_tmpTag.indexOf("ar:") != -1) {
                        _pointPos = _tmpTag.indexOf(":");
                        _this.artist = _tmpTag.substring(_pointPos + 1)
                    } else if (_tmpTag.indexOf("ti:") != -1) {
                        _pointPos = _tmpTag.indexOf(":");
                        _this.songTitle = _tmpTag.substring(_pointPos + 1)
                    } else if (_tmpTag.indexOf("by:") != -1) {
                        _pointPos = _tmpTag.indexOf(":");
                        _this.byBody = _tmpTag.substring(_pointPos + 1)
                    } else if (_tmpTag.indexOf("offset:") != -1) {
                        _pointPos = _tmpTag.indexOf(":");
                        _this.offset = _tmpTag.substring(_pointPos + 1)
                    } else {
                        _tmpTag = _tmpTag.indexOf(".") != -1 ? _tmpTag: _tmpTag + ".00";
                        _rSubstr = _rSubstr.trim() == "" ? "......": _rSubstr.trim();
                        var _t = parseInt(_this.playTime2ms(_tmpTag) - _this.offset, 10);
                        _tmpTag = _this.ms2playTime(_t);
                        _this.lrcList.push({
                            time: _tmpTag,
                            context: _rSubstr
                        })
                    }
                }
            }
            _this.sortLrcList()
        };
        _this.sortLrcList = function() {
            _this.lrcList.sort(sortFunc)
        };
        _this.printLrcLines = function() {
            var _list = _this.lrcList;
            if ( !! _list) {
                var _len = _list.length,
                i = 0,
                _html = [];
                for (; i < _len; i++) _html.push('<p id="line_' + i + '"><span>' + _list[i].context + "</span></p>");
                return _html.join("")
            }
            return ""
        };
        _this.clearLrcInfo = function() {
            _this.haveLrc = false;
            _this.lrcList = [];
            _this.lyricData = "";
            _this.artist = "";
            _this.songTitle = "";
            _this.album = "";
            _this.byBody = "";
            _this.offset = 0;
            _this.preLrcItemIndex = -1;
            _this.lrcItemIndex = 0;
            _this.reqFlag = -1;
            _this.stopAnimFrame()
        };
        _this.clearLrcItem = function() {
            _this.lrcItem = {
                time: "00:00.00",
                context: ""
            }
        };
        _this.getLyricLineItemByLineIndex = function(lineIndex) {
            var _len = _this.lrcList.length;
            if (lineIndex < 0 || lineIndex >= _len) return false;
            _this.clearLrcItem();
            _this.lrcItemIndex < lineIndex && (_this.preLrcItemIndex = _this.lrcItemIndex);
            _this.lrcItemIndex = lineIndex;
            _this.lrcItem = _this.lrcList[_this.lrcItemIndex];
            return true
        };
        _this.getLyricLineItemByTimes = function(minutes, seconds, mSecond) {
            if (minutes < 0 || (seconds < 0 || mSecond < 0)) return false;
            _this.clearLrcItem();
            var _curPlayTime = minutes + ":" + seconds + "." + mSecond;
            return _this.getLyricLineItemByPlayTime(_curPlayTime)
        };
        _this.getLyricLineItemByPlayTime = function(playTime) {
            _this.clearLrcItem();
            var _curPlayTime = playTime,
            _lrcItemCount = _this.lrcList.length,
            _lineIndex = 0,
            i = 0;
            if (_lrcItemCount <= 0) return false;
            for (; i < _lrcItemCount - 1; i++) {
                if (0 == i && _this.compare(_curPlayTime, _this.lrcList[i].time, "<")) return _this.getLyricLineItemByLineIndex(0);
                var _startItem = _this.lrcList[i],
                _endItem = _this.lrcList[i + 1];
                if (_this.compare(_startItem.time, _curPlayTime, "<=") && _this.compare(_curPlayTime, _endItem.time, "<")) {
                    _lineIndex = i;
                    return _this.getLyricLineItemByLineIndex(_lineIndex)
                }
            }
            if (_this.compare(_this.lrcList[_lrcItemCount - 1].time, _curPlayTime)) {
                _lineIndex = _lrcItemCount - 1;
                return _this.getLyricLineItemByLineIndex(_lineIndex)
            }
            return false
        };
        _this.playTime2ms = function(playTime) {
            var _minutes = parseInt(playTime.substring(0, playTime.indexOf(":")), 10) * 60 * 1E3,
            _seconds = parseInt(playTime.substring(playTime.indexOf(":") + 1, playTime.indexOf(".")), 10) * 1E3,
            _mSecond = parseInt(playTime.substring(playTime.indexOf(".") + 1), 10);
            return _minutes + _seconds + _mSecond
        };
        _this.ms2playTime = function(ms) {
            var _minutes = parseInt(ms / 6E4, 10),
            _seconds = parseInt(ms / 1E3 % 60, 10),
            _mSecond = ms - _minutes * 6E4 - _seconds * 1E3;
            return (_minutes > 9 ? "": "0") + _minutes + ":" + (_seconds > 9 ? "": "0") + _seconds + "." + (_mSecond > 9 ? "": "0") + _mSecond
        };
        _this.compare = function(pt1, pt2, opt) {
            opt = opt || "<=";
            if ("<" == opt) return _this.playTime2ms(pt1) < _this.playTime2ms(pt2);
            else if ("<=" == opt) return _this.playTime2ms(pt1) <= _this.playTime2ms(pt2);
            else if (">" == opt) return _this.playTime2ms(pt1) > _this.playTime2ms(pt2);
            else if (">=" == opt) return _this.playTime2ms(pt1) >= _this.playTime2ms(pt2)
        };
        _this.scrollLine = function(elem) {
            elem = typeof elem == "string" ? $D.get(elem) : elem;
            var max = parseInt(elem.scrollHeight - 72),
            _top = elem.scrollTop,
            _line_height = !!$D.get("line_" + g_lrcHandlerChn.lrcItemIndex) ? $D.get("line_" + g_lrcHandlerChn.lrcItemIndex).offsetHeight: 24,
            stop = false,
            _idx = g_lrcHandlerChn.lrcItemIndex || 0,
            heights = 0,
            _times = parseInt(_line_height / 6, 10),
            _off = _line_height % 6;
            while (_idx-->0) heights += !!$D.get("line_" + _idx) ? $D.get("line_" + _idx).offsetHeight: 24;
            stop = _top == heights;
            if (!stop) if (heights - _top >= _line_height * 2) elem.scrollTop = heights;
            else {
                var _diff = heights - _top;
                if (_diff > _line_height) _diff -= _line_height;
                var _t = _diff >= _times ? _times: _off;
                elem.scrollTop = _top + _t
            }
            g_lrcHandlerChn.startAnimFrame(elem)
        };
        _this.aniFrame = null;
        _this.lrcAnimFrame = function(cb, delay) {
            return window.requestTimeout(cb, delay)
        };
        _this.startAnimFrame = function(elem) {
            g_lrcHandlerChn.aniFrame = g_lrcHandlerChn.lrcAnimFrame(function() {
                g_lrcHandlerChn.scrollLine(elem)
            },
            delay)
        };
        _this.stopAnimFrame = function() {
            if (g_lrcHandlerChn.aniFrame != null) window.cancelRequestAnimFrame(g_lrcHandlerChn.aniFrame);
            g_lrcHandlerChn.aniFrame = null
        };
        _this.restartAnimFrame = function(elem) {
            g_lrcHandlerChn.stopAnimFrame();
            g_lrcHandlerChn.startAnimFrame(elem)
        }
    }
} ();
var g_lrcHandlerChn = new MUSIC.channel.lrcHandler;
/*  |xGv00|78e159b8af86505ece8f0d693a5fe58b */
